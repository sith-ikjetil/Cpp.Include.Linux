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
#include <unistd.h>
#include <fcntl.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>

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
                fcntl(this->m_socketfd, F_SETFL, O_NONBLOCK);
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
                fcntl(this->m_socketfd, F_SETFL, O_NONBLOCK);
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
                fcntl(this->m_socketfd, F_SETFL, O_NONBLOCK);
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
}// ItSoftware::Linux::IPC