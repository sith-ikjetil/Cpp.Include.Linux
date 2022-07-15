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
    // class: ItsSocketPassive
    //
    // (i): Class for passive socket communications.
    //      Allows for listen and response.
    //
    class ItsSocketPassive
    {
    private:
        ItsSocketDomain m_domain;
        ItsSocketConType m_type;
        const struct sockaddr* m_addr;
        socklen_t m_addrlen;
        int m_backlog;
        int m_socketfd;
        bool m_bInitWithError;
        bool m_bIsClosed;
    protected:
    public:
        //
        // Constructor: ItsNetPassive
        //
        // (i): Passive socket constructor. After constructor check GetInitWithError if all is well.
        //
        ItsSocketPassive(ItsSocketDomain domain, ItsSocketConType type, const struct sockaddr *addr, socklen_t addrlen, int backlog)
            : m_domain(domain),
            m_type(type),
            m_addr(addr),
            m_backlog(backlog),
            m_addrlen(addrlen),
            m_bInitWithError(true),
            m_bIsClosed(false)
        {
            this->m_socketfd = socket(this->m_domain, this->m_type, 0);
            if (this->m_socketfd >= 0) {
                fcntl(this->m_socketfd, F_SETFL, O_NONBLOCK);
                int retval = bind(this->m_socketfd, addr, addrlen);
                if (retval == 0) {
                    retval = listen(this->m_socketfd,backlog);
                    if (retval == 0) {
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
        // Destructor: ~ItsNetPassive
        //
        // (i): Destructor.
        //
        ~ItsSocketPassive()
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
    class ItsSocketActive
    {
    private:
        ItsSocketDomain m_domain;
        ItsSocketConType m_type;
        const struct sockaddr* m_addr;
        socklen_t m_addrlen;
        int m_backlog;
        int m_socketfd;
        bool m_bInitWithError;
        bool m_bIsClosed;
    protected:
    public:
        //
        // Constructor: ItsNetPassive
        //
        // (i): Passive socket constructor. After constructor check GetInitWithError if all is well.
        //
        ItsSocketActive(ItsSocketDomain domain, ItsSocketConType type, const struct sockaddr *addr, socklen_t addrlen)
            : m_domain(domain),
            m_type(type),
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
                this->Close();
            }
        }
        //
        // Destructor: ~ItsNetPassive
        //
        // (i): Destructor.
        //
        ~ItsSocketActive()
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
        // Method: GetIsClosed
        //
        // (i): Returnes bool if class is closed.
        //
        bool GetIsClosed()
        {
            return this->m_bIsClosed;
        }
    };
}// ItSoftware::Linux::IPC