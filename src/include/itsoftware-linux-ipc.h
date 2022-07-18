///////////////////////////////////////////////////////////////////
//: Title        : itsoftware-linux-ipc.h
//: Product:     : Cpp.Include.Linux
//: Date         : 2022-07-15
//: Author       : "Kjetil Kristoffer Solberg" <post@ikjetil.no>
//: Version      : 1.0.0.0
//: Descriptions : Implementation of Cpp.Include.Linux.
//: Uses         :
#pragma once
//
// #include
//
#include <string>
#include <memory>
#include <unistd.h>
#include <fcntl.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/msg.h>

//
// using
//
using std::string;
using std::to_string;
using std::unique_ptr;
using std::make_unique;

//
// namespace
//
namespace ItSoftware::Linux::IPC
{
    //
    // enum ItsSocketDomain
    //
    // (i): Socket domain.
    //
    enum ItsSocketDomain : int
    {
        UNIX = AF_UNIX,
        INET = AF_INET,
        INET6 = AF_INET6
    };

    //
    // enum: ItsSocketConType
    //
    // (i): Socket type. 
    //
    enum ItsSocketConType : int
    {
        STREAM = SOCK_STREAM,
        DGRAM = SOCK_DGRAM
    };

    //
    // class: ItsSocketStreamServer
    //
    // (i): Class for passive socket communications.
    //      Allows for listen and response.
    //
    class ItsSocketStreamServer
    {
    private:
        ItsSocketDomain m_domain;
        ItsSocketConType m_type;
        const struct sockaddr* m_addr;
        socklen_t m_addrlen;
        int m_backlog;
        int m_socketfd;
        int m_errno;
        bool m_bInitWithError;
        bool m_bIsClosed;
    protected:
    public:
        //
        // Default backdrop for this class.
        //
        constexpr static int DefaultBackdrop = 5;
        //
        // Constructor: ItsSocketStreamServer
        //
        // (i): Passive socket constructor. After constructor check GetInitWithError if all is well.
        //
        ItsSocketStreamServer(ItsSocketDomain domain, const struct sockaddr *addr, socklen_t addrlen, int backlog)
            : m_domain(domain),
            m_errno(0),
            m_type(ItsSocketConType::STREAM),
            m_addr(addr),
            m_backlog(backlog),
            m_addrlen(addrlen),
            m_bInitWithError(true),
            m_bIsClosed(false)
        {
            this->m_socketfd = socket(this->m_domain, this->m_type, 0);
            if (this->m_socketfd >= 0) {
                fcntl(this->m_socketfd, F_SETFL, fcntl(this->m_socketfd, F_GETFL) | O_NONBLOCK);
                this->m_errno = bind(this->m_socketfd, addr, addrlen);
                if (this->m_errno == 0) {
                    this->m_errno = listen(this->m_socketfd,backlog);
                    if (this->m_errno == 0) {
                        this->m_bInitWithError = false;
                    }
                    else {
                        this->Close();
                    }
                }
                else {
                    this->Close();
                }
            }
            else {
                this->m_errno = errno;
                this->Close();
            }
        }
        //
        // Destructor: ~ItsSocketStreamServer
        //
        // (i): Destructor.
        //
        ~ItsSocketStreamServer()
        {
            this->Close();
        }
        //
        // Method: Close
        //
        // (i): Cleans up resources consumed by this class instance.
        //
        void Close()
        {
            if ( !this->m_bIsClosed ) {
                if (this->m_socketfd >= 0) {
                    close(this->m_socketfd);
                    this->m_socketfd = -1;
                }
                this->m_bIsClosed = true;
            }
        }
        //
        // Method: Accept
        //
        // (i): Listens and returnes when new connection has been accepted.
        //
        int Accept(struct sockaddr* addr, socklen_t* addrlen)
        {
            return accept(this->m_socketfd, addr, addrlen);
        }
        //
        // Method: Read
        //
        // (i): Reads from client.
        //
        ssize_t Read(int fd, void* buf, size_t n)
        {
            return read(fd, buf, n);
        }
        //
        // Method: Write
        //
        // (i): Writes to client.
        //
        ssize_t Write(int fd, const void* buf, size_t n)
        {
            return write(fd, buf, n);
        }
        //
        // Method: GetInitWithError
        //
        // (i): Return if ItsNetPassive initialized with or without error.
        //      If with error, then object not usable.
        //
        bool GetInitWithError()
        {
            return this->m_bInitWithError;
        }
        //
        // Method: GetInitWithErrorErrno
        //
        // (i): Returnes error number for init with error.
        //
        bool GetInitWithErrorErrno()
        {
            return this->m_errno;
        }
        //
        // Method: GetIsClosed
        //
        // (i): Returnes bool if class instance is closed or not. If closed cannot be used.
        //
        bool GetIsClosed()
        {
            return this->m_bIsClosed;
        }
    };// ItsNetPassive

    //
    // class: ItsSocketActive
    //
    // (i): Class for active socket communication.
    //
    class ItsSocketStreamClient
    {
    private:
        ItsSocketDomain m_domain;
        ItsSocketConType m_type;
        const struct sockaddr* m_addr;
        socklen_t m_addrlen;
        int m_backlog;
        int m_socketfd;
        int m_errno;
        bool m_bInitWithError;
        bool m_bIsClosed;
    protected:
    public:
        //
        // Constructor: ItsNetPassive
        //
        // (i): Passive socket constructor. After constructor check GetInitWithError if all is well.
        //
        ItsSocketStreamClient(ItsSocketDomain domain, const struct sockaddr *addr, socklen_t addrlen)
            : m_domain(domain),
            m_errno(0),
            m_type(ItsSocketConType::STREAM),
            m_addr(addr),
            m_addrlen(addrlen),
            m_bInitWithError(true),
            m_bIsClosed(false)
        {
            this->m_socketfd = socket(this->m_domain, this->m_type, 0);
            if (this->m_socketfd >= 0) {
                this->m_bInitWithError = false;
            }
            else {
                this->m_errno = errno;
                this->Close();
            }
        }
        //
        // Destructor: ~ItsNetPassive
        //
        // (i): Destructor.
        //
        ~ItsSocketStreamClient()
        {
            this->Close();
        }
        //
        // Method: Connect
        //
        // (i): Connects to remote server.
        //
        int Connect(struct sockaddr *addr, socklen_t addrlen)
        {
            return connect(this->m_socketfd, addr, addrlen);
        }
        //
        // Method: Close
        //
        // (i): Cleans up resources consumed by this class instance.
        //
        void Close()
        {
            if ( !this->m_bIsClosed ) {
                if (this->m_socketfd >= 0) {
                    close(this->m_socketfd);
                    this->m_socketfd = -1;
                }
                this->m_bIsClosed = true;
            }
        }
        //
        // Method: Read
        //
        // (i): Reads from client.
        //
        ssize_t Read(void* buf, size_t n)
        {
            return read(this->m_socketfd, buf, n);
        }
        //
        // Method: Write
        //
        // (i): Writes to client.
        //
        ssize_t Write(const void* buf, size_t n)
        {
            return write(this->m_socketfd, buf, n);
        }
        //
        // Method: GetInitWithError
        //
        // (i): Returnes bool if initialization errored.
        //
        bool GetInitWithError()
        {
            return this->m_bInitWithError;
        }
        //
        // Method: GetInitWithErrorErrno
        //
        // (i): Returnes error number for init with error.
        //
        bool GetInitWithErrorErrno()
        {
            return this->m_errno;
        }
        //
        // Method: GetIsClosed
        //
        // (i): Returnes bool if class is closed.
        //
        bool GetIsClosed()
        {
            return this->m_bIsClosed;
        }
    };
    //
    // class: ItsSocketDatagramServer
    //
    // (i): Class for datagram socket communications.
    //      Allows for listen and response.
    //
    class ItsSocketDatagramServer
    {
    private:
        ItsSocketDomain m_domain;
        ItsSocketConType m_type;
        const struct sockaddr* m_addr;
        socklen_t m_addrlen;
        int m_socketfd;
        int m_errno;
        bool m_bInitWithError;
        bool m_bIsClosed;
    protected:
    public:
        //
        // Constructor: ItsSocketDatagramServer
        //
        // (i): Passive socket constructor. After constructor check GetInitWithError if all is well.
        //
        ItsSocketDatagramServer(ItsSocketDomain domain, const struct sockaddr *addr, socklen_t addrlen)
            : m_domain(domain),
            m_errno(0),
            m_type(ItsSocketConType::DGRAM),
            m_addr(addr),
            m_addrlen(addrlen),
            m_bInitWithError(true),
            m_bIsClosed(false)
        {
            this->m_socketfd = socket(this->m_domain, this->m_type, 0);
            if (this->m_socketfd >= 0) {
                fcntl(this->m_socketfd, F_SETFL, fcntl(this->m_socketfd, F_GETFL) | O_NONBLOCK);
                this->m_errno = bind(this->m_socketfd, addr, addrlen);
                if (this->m_errno == 0) {
                    this->m_bInitWithError = false;
                }
                else {
                    this->Close();
                }
            }
            else {
                this->m_errno = errno;
                this->Close();
            }
        }
        //
        // Destructor: ~ItsNetPassive
        //
        // (i): Destructor.
        //
        ~ItsSocketDatagramServer()
        {
            this->Close();
        }
        //
        // Method: Close
        //
        // (i): Cleans up resources consumed by this class instance.
        //
        void Close()
        {
            if ( !this->m_bIsClosed ) {
                if (this->m_socketfd >= 0) {
                    close(this->m_socketfd);
                    this->m_socketfd = -1;
                }
                this->m_bIsClosed = true;
            }
        }
        //
        // Method: Read
        //
        // (i): Reads from client.
        //
        ssize_t RecvFrom(void* buf, size_t n, int flags, struct sockaddr* src_addr, socklen_t* addrlen)
        {
            return recvfrom(this->m_socketfd, buf, n, flags, src_addr, addrlen);
        }
        //
        // Method: Write
        //
        // (i): Writes to client.
        //
        ssize_t SendTo(const void* buf, size_t n, int flags, const struct sockaddr* dest_addr, socklen_t addrlen)
        {
            return sendto(this->m_socketfd, buf, n, flags, dest_addr, addrlen);
        }
        //
        // Method: GetInitWithError
        //
        // (i): Return if ItsNetPassive initialized with or without error.
        //      If with error, then object not usable.
        //
        bool GetInitWithError()
        {
            return this->m_bInitWithError;
        }
        //
        // Method: GetInitWithErrorErrno
        //
        // (i): Returnes error number for init with error.
        //
        bool GetInitWithErrorErrno()
        {
            return this->m_errno;
        }
        //
        // Method: GetIsClosed
        //
        // (i): Returnes bool if class instance is closed or not. If closed cannot be used.
        //
        bool GetIsClosed()
        {
            return this->m_bIsClosed;
        }
    };// ItsNetPassive

    //
    // class: ItsSocketActive
    //
    // (i): Class for active socket communication.
    //
    class ItsSocketDatagramClient
    {
    private:
        ItsSocketDomain m_domain;
        ItsSocketConType m_type;
        const struct sockaddr* m_addr;
        socklen_t m_addrlen;
        int m_backlog;
        int m_errno;
        int m_socketfd;
        bool m_bInitWithError;
        bool m_bIsClosed;
    protected:
    public:
        //
        // Constructor: ItsNetPassive
        //
        // (i): Datagram socket constructor. After constructor check GetInitWithError if all is well.
        //
        ItsSocketDatagramClient(ItsSocketDomain domain, const struct sockaddr *addr, socklen_t addrlen)
            : m_domain(domain),
            m_errno(0),
            m_type(ItsSocketConType::DGRAM),
            m_addr(addr),
            m_addrlen(addrlen),
            m_bInitWithError(true),
            m_bIsClosed(false)
        {
            this->m_socketfd = socket(this->m_domain, this->m_type, 0);
            if (this->m_socketfd >= 0) {
                fcntl(this->m_socketfd, F_SETFL, fcntl(this->m_socketfd, F_GETFL) | O_NONBLOCK);
                this->m_errno = bind(this->m_socketfd, addr, addrlen);
                if (this->m_errno == 0) {
                    this->m_bInitWithError = false;
                }
                else {
                    this->Close();
                }
            }
            else {
                this->m_errno = errno;
                this->Close();
            }
        }
        //
        // Destructor: ~ItsNetPassive
        //
        // (i): Destructor.
        //
        ~ItsSocketDatagramClient()
        {
            this->Close();
        }
        //
        // Method: Close
        //
        // (i): Cleans up resources consumed by this class instance.
        //
        void Close()
        {
            if ( !this->m_bIsClosed ) {
                if (this->m_socketfd >= 0) {
                    close(this->m_socketfd);
                    this->m_socketfd = -1;
                }
                this->m_bIsClosed = true;
            }
        }
        //
        // Method: Read
        //
        // (i): Reads from client.
        //
        ssize_t RecvFrom(void* buf, size_t n, int flags, struct sockaddr* src_addr, socklen_t* addrlen)
        {
            return recvfrom(this->m_socketfd, buf, n, flags, src_addr, addrlen);
        }
        //
        // Method: Write
        //
        // (i): Writes to client.
        //
        ssize_t SendTo(const void* buf, size_t n, int flags, const struct sockaddr* dest_addr, socklen_t addrlen)
        {
            return sendto(this->m_socketfd, buf, n, flags, dest_addr, addrlen);
        }
        //
        // Method: GetInitWithError
        //
        // (i): Returnes bool if initialization errored.
        //
        bool GetInitWithError()
        {
            return this->m_bInitWithError;
        }
        //
        // Method: GetInitWithErrorErrno
        //
        // (i): Returnes error number for init with error.
        //
        bool GetInitWithErrorErrno()
        {
            return this->m_errno;
        }
        //
        // Method: GetIsClosed
        //
        // (i): Returnes bool if class is closed.
        //
        bool GetIsClosed()
        {
            return this->m_bIsClosed;
        }
    };
    //
    // class: ItsPipe
    //
    // (i): Pipe file descriptor wrapper.
    //
    class ItsPipe
    {
    private:
        int m_fd[2];
        bool m_bInitWithError;
        int m_errno;
        bool m_bIsReadClosed;
        bool m_bIsWriteClosed;
    protected:
    public:
        //
        // Max buffer size
        //
        constexpr static int MaxBufferSize = PIPE_BUF;
        //
        // Method: Constructor
        //
        // (i): Constructs and initializes ItsPipe object.
        //
        ItsPipe()
            : m_bInitWithError(true),
            m_errno(0),
            m_bIsReadClosed(false),
            m_bIsWriteClosed(false) 
        {
            this->m_errno = pipe2(this->m_fd,O_DIRECT|O_NONBLOCK);
            if ( this->m_errno != -1 ) {
                this->m_bInitWithError = false;
            }
        }
        //
        // Method: ~ItsPipe
        //
        // (i): Destructor.
        //
        ~ItsPipe()
        {
            this->Close();
        }
        //
        // Method: GetInitWithError
        //
        // (i): Returnes true if initialization failed.
        //
        bool GetInitWithError()
        {
            return this->m_bInitWithError;
        }
        //
        // Method: GetInitWithErrorErrnum
        //
        // (i): Returns initialization error code.
        //
        int GetInitWithErrorErrno()
        {
            return this->m_errno;
        }
        //
        // Method: GetIsReadClosed
        //
        // (i): Returnes true if read file descriptor is closed.
        //
        bool GetIsReadClosed()
        {
            return this->m_bIsReadClosed;
        }
        //
        // Method: GetIsWriteClosed
        //
        // (i): Returnes true if write file descriptor is closed.
        //
        bool GetIsWriteClosed()
        {
            return this->m_bIsWriteClosed;
        }
        //
        // Method: CloseRead
        //
        // (i): Closes read descriptor.
        //
        void CloseRead()
        {
            if (!this->GetInitWithError()) {
                if ( close(this->m_fd[0]) == 0 ) {
                    this->m_bIsReadClosed = true;
                }
            }
        }
        //
        // Method: CloseWrite
        //
        // (i): Closes write file descriptor.
        //
        void CloseWrite()
        {
            if (!this->GetInitWithError()) {
                if ( close(this->m_fd[1]) == 0 ) {
                    this->m_bIsWriteClosed = true;
                }
            }
        }
        //
        // Method: Close
        //
        // (i): Closes both read- and write -file descriptors.
        //
        void Close()
        {
            this->CloseRead();
            this->CloseWrite();
        }
        //
        // Method: Write
        //
        // (i): Writes to write file descriptor.
        //
        ssize_t Write(void* buf, size_t n)
        {
            if (!this->GetIsWriteClosed() ) {
                return write(this->m_fd[1], buf, n);
            }
            return -1;
        }
        //
        // Method: Read
        //
        // (i): Reads from read file descriptor.
        //
        ssize_t Read(void* buf, size_t n)
        {
            if (!this->GetIsReadClosed() ) {
                return read(this->m_fd[0], buf, n);
            }
            return -1;
        }
    };
    //
    // enum ItsSvMsgFlags
    //
    // (i): 
    //
    enum class ItsSvMsgFlags : int
    {
        MF_NONE = 0,
        MF_IPC_NOWAIT = IPC_NOWAIT // do not block
    };
    //
    // struct: ItsSvMsg[1,2,4,8]k
    // 
    // (i): default message structs for some given sizes.
    //
    struct ItsSvMsg1k {
       long mtype;
       char mtext[1024];
    };
    struct ItsSvMsg2k {
       long mtype;
       char mtext[2048];
    };
    struct ItsSvMsg4k {
       long mtype;
       char mtext[4096];
    };
    struct ItsSvMsg8k {
       long mtype;
       char mtext[8192];
    };
    //
    // class: ItsSvMsgQueue
    //
    // (i): System V IPC Message Queue
    //
    class ItsSvMsgQueue
    {
    private:
        key_t m_key;
        int m_msqid;
        bool m_bIsDeleted;
        bool m_bInitWithError;
        int m_errno;
    protected:
    public:
        //
        // Function: CreateQueueKey
        //
        // (i): Creates a queue key from path and project id.
        //      Return value of -1 means failure. errno indicates error.
        //
        static key_t CreateQueueKey(const char* path, char projid)
        {
            return ftok(path, (int)projid);
        } 
        //
        // Method: Constructor
        //
        // (i): Constructor.
        //
        // The  msgget() system call returns the System V message queue identifier
        // associated with the value of the key argument.  It may be  used  either
        // to  obtain  the  identifier of a previously created message queue (when
        // msgflg is zero and key does not have the value IPC_PRIVATE), or to cre‐
        // ate a new set.
        //
        // A  new message queue is created if key has the value IPC_PRIVATE or key
        // isn't IPC_PRIVATE, no message queue with the given key key exists,  and
        // IPC_CREAT is specified in msgflg.
        //
        // If msgflg specifies both IPC_CREAT and IPC_EXCL and a message queue al‐
        // ready exists for key, then msgget() fails with  errno  set  to  EEXIST.
        // (This  is  analogous  to the effect of the combination O_CREAT | O_EXCL
        // for open(2).)
        //
        // (a): key=IPC_PRIVATE or ItsSvMsgQueue::CreateQueueKey.
        //
        ItsSvMsgQueue(key_t key, int flags)
            : m_key(key),
            m_msqid(-1),
            m_bIsDeleted(false), 
            m_bInitWithError(true),
            m_errno(0)
        {
            this->m_msqid = msgget(this->m_key, flags);
            if ( this->m_msqid != -1) {
                this->m_bInitWithError = false;
            }
            else {
                this->m_errno = errno;
            }
        }
        //
        // Method: GetInitWithError
        //
        // (i): Returnes true if initialization failed.
        //
        bool GetInitWithError()
        {
            return this->m_bInitWithError;
        }
        //
        // Method: GetInitWithErrorErrnum
        //
        // (i): Returns initialization error code.
        //
        int GetInitWithErrorErrno()
        {
            return this->m_errno;
        }
        //
        // Method: MsgSnd
        //
        // (i): Sends a message.
        //
        int MsgSnd(const void* msg, size_t msgsz, int msgflags)
        {
            return msgsnd(this->m_msqid, (const void*)msg, msgsz, msgflags);
        }
        //
        // Method: MsgRcv
        //
        // (i): Sends a message.
        //
        ssize_t MsgRcv(void* msg, size_t maxmsgsz, long msgtype, int msgflags)
        {
            return msgrcv(this->m_msqid, (void*)msg, maxmsgsz, msgtype, msgflags);
        }
        //
        // Method: Delete
        //
        // (i): Deletes message queue.
        //
        int Delete()
        {
            if (!this->m_bIsDeleted) {
                int retval = msgctl(this->m_msqid, IPC_RMID, 0);
                if (retval != -1) {
                    this->m_bIsDeleted = true;
                } 
                return retval;
            }
            return -1;
        }
        //
        // Method: GetIsDelted
        //
        // (i): Gets flag if queue has been deleted.
        //
        bool GetIsDeleted()
        {
            return this->m_bIsDeleted;
        }
        //
        // Method: GetMessageQueueId
        //
        // (i): Returnes message queue id.
        //
        int GetMessageQueueId()
        {
            return this->m_msqid;
        }
        //
        // Function: CreateFlags
        //
        // (i): Creates constructor flags.
        //
        static int CreateQueueFlags(bool ipc_creat, bool ipc_excl, string user, string group, string other)
        {
            int flags(0);

            if ( ipc_creat ) { flags |= IPC_CREAT; }
            if ( ipc_excl ) { flags |= IPC_EXCL; }

            if (user.size() > 0)
            {
                if (user.find('r',0) != string::npos) {
                    flags |= S_IRUSR;
                }
                if (user.find('w',0) != string::npos) {
                    flags |= S_IWUSR;
                }
                if (user.find('x',0) != string::npos) {
                    flags |= S_IXUSR;
                }
            }

            if (group.size() > 0)
            {
                if (group.find('r',0) != string::npos) {
                    flags |= S_IRGRP;
                }
                if (group.find('w',0) != string::npos) {
                    flags |= S_IWGRP;
                }
                if (group.find('x',0) != string::npos) {
                    flags |= S_IXGRP;
                }
            }

            if (other.size() > 0)
            {
                if (other.find('r',0) != string::npos) {
                    flags |= S_IROTH;
                }
                if (other.find('w',0) != string::npos) {
                    flags |= S_IWOTH;
                }
                if (other.find('x',0) != string::npos) {
                    flags |= S_IXOTH;
                }
            }

            return flags;
        }
        //
        // Function: CreateMsgFlags
        //
        // (i): Create flags used in msgsnd and msgrcv.
        //
        static int CreateMsgFlags(bool ipc_nowait, bool msg_copy, bool msg_except, bool msg_noerror) 
        {
            int flags(0);

            if (ipc_nowait) { flags |= IPC_NOWAIT; }
            if (msg_copy) { flags |= MSG_COPY; }
            if (msg_except) { flags |= MSG_EXCEPT; }
            if (msg_noerror) { flags |= MSG_NOERROR; }

            return flags;
        }
    };
    //
    // struct: ItsFifoHeader
    //
    // (i): Predetermined header for each read/write.
    //
    struct ItsFifoHeader {
        pid_t  pid;
        size_t length;
    };
    //
    // class: ItsFifoServer
    //
    // (i): Implementation of FIFO Server IPC.
    //
    class ItsFifoServer
    {
    private:
        int m_serverfd;
        int m_dummyfd;
        int m_errno;
        string m_server_filename;
        string m_client_filename;
        bool m_bInitWithError;
        bool m_bIsClosed;
    protected:
    public:
        ItsFifoServer(const string server_filename, const string client_filename, int flags)
            : m_serverfd(-1),
            m_dummyfd(-1),
            m_errno(0),
            m_bIsClosed(false),
            m_server_filename(server_filename),
            m_client_filename(client_filename),
            m_bInitWithError(true)
        {
            umask(0);
            this->m_errno = mkfifo(this->m_server_filename.c_str(), flags);
            if (this->m_errno != -1) {
                this->m_serverfd = open(this->m_server_filename.c_str(), O_RDONLY | O_NONBLOCK);
                if (this->m_serverfd != -1) {
                    this->m_dummyfd = open(this->m_server_filename.c_str(), O_WRONLY | O_NONBLOCK);
                    if (this->m_dummyfd != -1) {
                        this->m_bInitWithError = false;
                    }
                    else {
                        this->Close();
                    }
                }
                else {
                    this->Close();
                }
            }
            else {
                this->Close();
            }
        }
        //
        // Method: ~ItsFifoServer
        //
        // (i): Destructor.
        //
        ~ItsFifoServer()
        {
            this->Close();
        }
        //
        // Method: Close
        //
        // (i): Closes open handles.
        //
        void Close()
        {
            if ( !this->m_bIsClosed ) {
                if ( this->m_dummyfd != -1 ) {
                    close(this->m_dummyfd);
                }
                if ( this->m_serverfd != -1 ) {
                    close(this->m_serverfd);
                }
                remove(this->m_server_filename.c_str());
                this->m_bIsClosed = true;
            }
        }
        //
        // Method: IsClosed
        //
        // (i): Returnes if object is closed.
        //
        bool GetIsClosed()
        {
            return this->m_bIsClosed;
        }
        //
        // Function: CreateServerFlags
        //
        // (i): Create flags for server creation.
        //
        static int CreateFifoFlags(string user, string group, string other)
        {
            int flags(0);

            if (user.size() > 0)
            {
                if (user.find('r',0) != string::npos) {
                    flags |= S_IRUSR;
                }
                if (user.find('w',0) != string::npos) {
                    flags |= S_IWUSR;
                }
                if (user.find('x',0) != string::npos) {
                    flags |= S_IXUSR;
                }
            }

            if (group.size() > 0)
            {
                if (group.find('r',0) != string::npos) {
                    flags |= S_IRGRP;
                }
                if (group.find('w',0) != string::npos) {
                    flags |= S_IWGRP;
                }
                if (group.find('x',0) != string::npos) {
                    flags |= S_IXGRP;
                }
            }

            if (other.size() > 0)
            {
                if (other.find('r',0) != string::npos) {
                    flags |= S_IROTH;
                }
                if (other.find('w',0) != string::npos) {
                    flags |= S_IWOTH;
                }
                if (other.find('x',0) != string::npos) {
                    flags |= S_IXOTH;
                }
            }

            return flags;
        }
        //
        // Method: GetInitWithError
        //
        // (i): Returnes true if initialization failed.
        //
        bool GetInitWithError()
        {
            return this->m_bInitWithError;
        }
        //
        // Method: GetInitWithErrorErrnum
        //
        // (i): Returns initialization error code.
        //
        int GetInitWithErrorErrno()
        {
            return this->m_errno;
        }
        //
        // Method: Read
        //
        // (i): Reads from fifo file.
        //
        ssize_t Read(unique_ptr<unsigned char[]> *buf, ItsFifoHeader* header)
        {
            auto nr = read(this->m_serverfd, header, sizeof(ItsFifoHeader));
            if ( nr == sizeof(ItsFifoHeader) ) {
                *buf = make_unique<unsigned char[]>(header->length);
                return read(this->m_serverfd, (*buf).get(), header->length);
            }

            return 0;
        }
        //
        // Method: Write
        //
        // (i): Writes to fifo file.
        //
        ssize_t Write(const void *buf, ItsFifoHeader* header)
        {
            string wfilename(this->m_client_filename);
            wfilename += ".";
            wfilename += to_string(header->pid);

            int clientfd = open(wfilename.c_str(), O_WRONLY | O_NONBLOCK);
            if ( clientfd == -1 ) {
                return -1;
            }
                
            auto nw = write(clientfd, header, sizeof(ItsFifoHeader));
            if ( nw == sizeof(ItsFifoHeader) ) {
                nw = write(clientfd, buf, header->length);
                close(clientfd);
                return nw;
            }

            return 0;
        }
    };
    //
    // class: ItsFifoClient
    //
    // (i): Implementation of FIFO Server IPC.
    //
    class ItsFifoClient
    {
    private:
        int m_clientfd;
        int m_dummyfd;
        int m_errno;
        string m_server_filename;
        string m_client_filename;
        bool m_bInitWithError;
        bool m_bIsClosed;
    protected:
    public:
        ItsFifoClient(const string server_filename, const string client_filename, int flags)
            : m_clientfd(-1),
            m_dummyfd(-1),
            m_errno(0),
            m_bIsClosed(false),
            m_server_filename(server_filename),
            m_client_filename(client_filename),
            m_bInitWithError(true)
        {
            this->m_client_filename += ".";
            this->m_client_filename += to_string(getpid());

            umask(0);
            this->m_errno = mkfifo(this->m_client_filename.c_str(), flags);
            if (this->m_errno != -1) {
                this->m_clientfd = open(this->m_client_filename.c_str(), O_RDONLY | O_NONBLOCK);
                if (this->m_clientfd != -1) {
                    this->m_dummyfd = open(this->m_server_filename.c_str(), O_WRONLY | O_NONBLOCK);
                    if (this->m_dummyfd != -1) {
                        this->m_bInitWithError = false;
                    }
                    else {
                        this->Close();
                    }
                }
                else {
                    this->Close();
                }
            }
            else {
                this->Close();
            }
        }
        //
        // Method: ~ItsFifoClient
        //
        // (i): Destructor.
        //
        ~ItsFifoClient()
        {
            this->Close();
        }
        //
        // Method: Close
        //
        // (i): Closes open handles.
        //
        void Close()
        {
            if ( !this->m_bIsClosed ) {
                if ( this->m_dummyfd != -1 ) {
                    close(this->m_dummyfd);
                }
                if ( this->m_clientfd != -1 ) {
                    close(this->m_clientfd);
                }
                remove(this->m_client_filename.c_str());
                this->m_bIsClosed = true;
            }
        }
        //
        // Method: IsClosed
        //
        // (i): Returnes if object is closed.
        //
        bool GetIsClosed()
        {
            return this->m_bIsClosed;
        }
        //
        // Function: CreateServerFlags
        //
        // (i): Create flags for server creation.
        //
        static int CreateFifoFlags(string user, string group, string other)
        {
            int flags(0);

            if (user.size() > 0)
            {
                if (user.find('r',0) != string::npos) {
                    flags |= S_IRUSR;
                }
                if (user.find('w',0) != string::npos) {
                    flags |= S_IWUSR;
                }
                if (user.find('x',0) != string::npos) {
                    flags |= S_IXUSR;
                }
            }

            if (group.size() > 0)
            {
                if (group.find('r',0) != string::npos) {
                    flags |= S_IRGRP;
                }
                if (group.find('w',0) != string::npos) {
                    flags |= S_IWGRP;
                }
                if (group.find('x',0) != string::npos) {
                    flags |= S_IXGRP;
                }
            }

            if (other.size() > 0)
            {
                if (other.find('r',0) != string::npos) {
                    flags |= S_IROTH;
                }
                if (other.find('w',0) != string::npos) {
                    flags |= S_IWOTH;
                }
                if (other.find('x',0) != string::npos) {
                    flags |= S_IXOTH;
                }
            }

            return flags;
        }
        //
        // Method: GetInitWithError
        //
        // (i): Returnes true if initialization failed.
        //
        bool GetInitWithError()
        {
            return this->m_bInitWithError;
        }
        //
        // Method: GetInitWithErrorErrnum
        //
        // (i): Returns initialization error code.
        //
        int GetInitWithErrorErrno()
        {
            return this->m_errno;
        }
        //
        // Method: Read
        //
        // (i): Reads from fifo file.
        //
        ssize_t Read(unique_ptr<unsigned char[]> *buf, ItsFifoHeader* header)
        {
            auto nr = read(this->m_clientfd, header, sizeof(ItsFifoHeader));
            if ( nr == sizeof(ItsFifoHeader) ) {
                *buf = make_unique<unsigned char[]>(header->length);
                return read(this->m_clientfd, (*buf).get(), header->length);
            }

            return 0;
        }
        //
        // Method: Write
        //
        // (i): Writes to fifo file.
        //
        ssize_t Write(const void *buf, ItsFifoHeader* header)
        {
            string wfilename(this->m_server_filename);
            
            int serverfd = open(wfilename.c_str(), O_WRONLY | O_NONBLOCK);
            if ( serverfd == -1 ) {
                return -1;
            }
                
            auto nw = write(serverfd, header, sizeof(ItsFifoHeader));
            if ( nw == sizeof(ItsFifoHeader) ) {
                nw = write(serverfd, buf, header->length);
                close(serverfd);
                return nw;
            }

            return 0;
        }
    };
}// ItSoftware::Linux::IPC