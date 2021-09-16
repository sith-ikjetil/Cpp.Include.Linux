///////////////////////////////////////////////////////////////////
//: Title        : itsoftware-linux-core.h
//: Product:     : Cpp.Include.Linux
//: Date         : 2020-05-01
//: Author       : "Kjetil Kristoffer Solberg" <post@ikjetil.no>
//: Version      : 1.0.0.0
//: Descriptions : Implementation of Cpp.Include.Linux.
#pragma once
//
// #include
//
#include <string>
#include <memory>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <iostream>
#include <vector>
#include "itsoftware-linux.h"

//
// namespace
//
namespace ItSoftware
{
    //
    // namespace
    //
    namespace Linux
    {
        //
        // namespace
        //
        namespace Core
        {
            //
            // using
            //
            using std::make_unique;
            using std::string;
            using std::unique_ptr;
            using std::vector;
            using ItSoftware::Linux::ItsString;

            //
            // struct: ItsTimer
            //
            struct ItsTimer
            {
              private:
                size_t m_start = 0;
                size_t m_end = 0;
                tms m_tms;
                bool m_isRunning = false;

                size_t GetTickCount()
                {
                    struct timespec ts;
                    if (clock_gettime(CLOCK_REALTIME,&ts) != 0)
                    {
                        return 0;
                    }

                    return ts.tv_nsec;
                }

              public:
                void Start()
                {
                    if (!this->IsRunning())
                    {
                        this->m_start = this->GetTickCount();
                        this->m_end = m_start;
                        this->m_isRunning = true;
                    }
                }

                void Stop()
                {
                    this->m_end = this->GetTickCount();
                    this->m_isRunning = false;
                }

                bool IsRunning()
                {
                    return this->m_isRunning;
                }

                size_t LapSeconds()
                {
                    if (!this->IsRunning())
                    {
                        return 0;
                    }

                    return (size_t)((this->GetTickCount() - this->m_start) / 1'000'000'000);
                }

                size_t LapMilliseconds()
                {
                    if (!this->IsRunning())
                    {
                        return 0;
                    }

                    return (size_t)((this->GetTickCount() - this->m_start) / 1'000'000);
                }

                size_t LapMicroseconds()
                {
                    if (!this->IsRunning())
                    {
                        return 0;
                    }

                    return (size_t)((this->GetTickCount() - this->m_start) / 1'000);
                }

                size_t LapNanoseconds()
                {
                	if (!this->IsRunning())
                	{
                		return 0;
                	}
                	return (this->GetTickCount() - this->m_start);
                }

                size_t GetSeconds()
                {
                    if (this->IsRunning())
                    {
                        return 0;
                    }

                    return (size_t)((this->m_end - this->m_start) / 1'000'000'000);
                }

                size_t GetMilliseconds()
                {
                    if (this->IsRunning())
                    {
                        return 0;
                    }

                    return (size_t)((this->m_end - this->m_start) / 1'000'000);
                }

                size_t GetMicroseconds()
                {
                    if (this->IsRunning())
                    {
                        return 0;
                    }

                    return (size_t)((this->m_end - this->m_start) / 1'000);
                }

                size_t GetNanoseconds()
                {
                	if (this->IsRunning())
                	{
                		return 0;
                	}
                	return (this->m_end - this->m_start);
                }
            };

            //
            // struct: unique_file_descriptor
            //
            struct unique_file_descriptor
            {
              private:
                int m_fd = -1;

              protected:
              public:
                unique_file_descriptor()
                {
                    this->m_fd = -1;
                }

                unique_file_descriptor(int fd)
                {
                    this->m_fd = fd;
                }

                ~unique_file_descriptor()
                {
                    this->Close();
                }

                bool IsValid() const
                {
                    return (this->m_fd >= 0);
                }

                bool IsInvalid() const
                {
                    return (this->m_fd < 0);
                }

                void operator=(int fd)
                {
                    this->Close();
                    this->m_fd = fd;
                }

                operator int()
                {
                    return this->m_fd;
                }

                bool Close()
                {
                    if (this->IsInvalid())
                    {
                        return false;
                    }
                    close(this->m_fd);
                    this->m_fd = -1;
                    return true;
                }

                int p() const
                {
                    return this->m_fd;
                }

                const int *GetAddressOf() const
                {
                    return &this->m_fd;
                }
            };

            //
            // File IO Wrapper
            //
            class ItsFile
            {
              private:
                unique_file_descriptor m_fd;
                string m_filename;
              protected:
              public:
                ItsFile()
                {
                }

                //
                // GetFilename
                //
                string GetFilename()
                {
	           		return this->m_filename;
                }

                //
                // Method: OpenExisting
                //
                bool OpenExisting(string filename, string flags)
                {
                    if (this->m_fd.IsValid())
                    {
                        return false;
                    }

                    if (filename.size() == 0)
                    {
                        return false;
                    }

                    if (flags.size() == 0)
                    {
                        return false;
                    }

                    if (!ItsFile::Exists(filename))
                    {
                        return false;
                    }

                    int i_flags = 0;
                    if (flags.find("r") != string::npos &&
                        flags.find("w") != string::npos)
                    {
                        i_flags |= O_RDWR;
                    }
                    else if (flags.find("r") != string::npos)
                    {
                        i_flags |= O_RDONLY;
                    }
                    else if (flags.find("w") != string::npos)
                    {
                        i_flags |= O_WRONLY;
                    }
                    else
                    {
                        return false;
                    }

                    i_flags |= O_CREAT;
                    i_flags |= O_LARGEFILE;

                    if (flags.find("t") != string::npos)
                    {
                        i_flags |= O_TRUNC;
                    }
                    else if (flags.find("a") != string::npos)
                    {
                        i_flags |= O_APPEND;
                    }

                    this->m_fd = open(filename.c_str(), i_flags);
                    if (this->m_fd.IsInvalid())
                    {
                        return false;
                    }

	            	this->m_filename = filename;
                    return true;
                }

                //
                // Method: OpenOrCreate
                //
                // (i) mode = "rwta" (read, write, trunc, append)
                //
                bool OpenOrCreate(string filename, string flags, int mode)
                {
                    if (this->m_fd.IsValid())
                    {
                        return false;
                    }

                    if (filename.size() == 0)
                    {
                        return false;
                    }

                    if (flags.size() == 0)
                    {
                        return false;
                    }

                    int i_flags = 0;
                    if (flags.find("r") != string::npos &&
                        flags.find("w") != string::npos)
                    {
                        i_flags |= O_RDWR;
                    }
                    else if (flags.find("r") != string::npos)
                    {
                        i_flags |= O_RDONLY;
                    }
                    else if (flags.find("w") != string::npos)
                    {
                        i_flags |= O_WRONLY;
                    }
                    else
                    {
                        return false;
                    }

                    i_flags |= O_CREAT;
                    i_flags |= O_LARGEFILE;

                    if (flags.find("t") != string::npos)
                    {
                        i_flags |= O_TRUNC;
                    }
                    else if (flags.find("a") != string::npos)
                    {
                        i_flags |= O_APPEND;
                    }

                    this->m_fd = open(filename.c_str(), i_flags, mode);
                    if (this->m_fd.IsInvalid())
                    {
                        return false;
                    }

	            	this->m_filename = filename;

                    return true;
                }

                bool Write(const void *data, size_t bytesToWrite, size_t *bytesWritten)
                {
                    if (this->IsInvalid())
                    {
                        return false;
                    }

                    if (bytesToWrite == 0)
                    {
                        return false;
                    }

                    *bytesWritten = write(this->m_fd.p(), data, bytesToWrite);

                    return (*bytesWritten > 0);
                }

                bool Read(void *data, size_t bytesToRead, size_t *bytesRead)
                {
                    if (this->IsInvalid())
                    {
                        return false;
                    }

                    if (bytesToRead == 0)
                    {
                        return false;
                    }

                    *bytesRead = read(this->m_fd.p(), data, bytesToRead);

                    return true;
                }

                //
                // ReadAllText
                //
                bool ReadAllText(string& str)
                {
                    if (this->IsInvalid())
                    {
                        return false;
                    }

                    this->SetPosFromBeg(0);

                    std::stringstream ss;

                    char data[2048];
                    size_t bytesRead(0);
                    while (this->Read((void*)data, 2048, &bytesRead) && bytesRead > 0)
                    {
                        ss.write(data, bytesRead);
                    }

                    str = ss.str();
                    return true;
                }

                //
                // ReadAllTextLines
                //
                bool ReadAllTextLines(std::vector<string>& lines)
                {
                    if (this->IsInvalid())
                    {
                        return false;
                    }

                    this->SetPosFromBeg(0);

                    string str;
                    if (!this->ReadAllText(str)) 
                    {
                        return false;
                    }

                    string split("\n");
                    lines = ItsString::Split(str, split);
                    return true;
                }

                bool SetPosFromBeg(off_t offset)
                {
                    if (this->IsInvalid())
                    {
                        return false;
                    }

                    if (lseek(this->m_fd, offset, SEEK_SET) == -1) 
                    {
                        return false;
                    }

                    return true;
                }

                bool SetPosFromEnd(off_t offset)
                {
                    if (this->IsInvalid())
                    {
                        return false;
                    }

                    if (lseek(this->m_fd, offset, SEEK_END) == -1) 
                    {
                        return false;
                    }

                    return true;
                }

                bool SetPosFromCur(off_t offset)
                {
                    if (this->IsInvalid())
                    {
                        return false;
                    }

                    if (lseek(this->m_fd, offset, SEEK_CUR) == -1) 
                    {
                        return false;
                    }

                    return true;
                }

                bool Close()
                {
                    return this->m_fd.Close();
                }

                bool IsValid()
                {
                    return this->m_fd.IsValid();
                }

                bool IsInvalid()
                {
                    return this->m_fd.IsInvalid();
                }

                operator int()
                {
                    return this->m_fd.p();
                }

                //
                // GetFileSize
                //
                static size_t GetFileSize(string filename)
                {
                    struct stat statbuf;
                    if ( stat(filename.c_str(), &statbuf) == -1 ) {
                        return -1;
                    }
                    return statbuf.st_size;
                }

                static bool Delete(string filename)
                {
                    return (!unlink(filename.c_str()));
                }

                static bool Exists(string filename)
                {
                    return (!access(filename.c_str(), F_OK));
                }

                static bool Move(string sourceFilename, string targetFilename)
                {
                    return (!rename(sourceFilename.c_str(), targetFilename.c_str()));
                }

                static bool Copy(string sourceFilename, string targetFilename, bool replaceIfExists)
                {
                    if (!ItsFile::Exists(sourceFilename))
                    {
                        return false;
                    }

                    ItsFile source;
                    source.OpenExisting(sourceFilename.c_str(), "r");
                    if (source.IsInvalid())
                    {
                        return false;
                    }

                    if (ItsFile::Exists(targetFilename) && !replaceIfExists)
                    {
                        return false;
                    }

                    ItsFile target;
                    if (ItsFile::Exists(targetFilename))
                    {
                        target.OpenExisting(targetFilename, "w");
                    }
                    else
                    {
                        int mode = 0;
                        ItsFile::Mode(sourceFilename, &mode);
                        target.OpenOrCreate(targetFilename.c_str(), "wt", mode);
                    }

                    if (target.IsInvalid())
                    {
                        return false;
                    }

                    const int buff_size = 8192;
                    unique_ptr<uint8_t[]> buffer = make_unique<uint8_t[]>(buff_size);
                    size_t bytes_read{0};
                    size_t bytes_written{0};
                    while (source.Read(buffer.get(), buff_size, &bytes_read) && bytes_read > 0)
                    {
                        target.Write(buffer.get(), bytes_read, &bytes_written);
                    }

                    return true;
                }

                static bool Mode(string filename, int *mode)
                {
                    if (!ItsFile::Exists(filename))
                    {
                        return false;
                    }

                    struct stat stat1;
                    if (stat(filename.c_str(), &stat1) != 0)
                    {
                        return false;
                    }
                    *mode = stat1.st_mode;
                    return true;
                }

                static int CreateMode(string user, string group, string other)
                {
                	int mode(0);

                	if (user.size() > 0)
                	{
                		if (user.find('r',0) != string::npos) {
							mode |= S_IRUSR;
                		}
                		if (user.find('w',0) != string::npos) {
							mode |= S_IWUSR;
                		}
                		if (user.find('x',0) != string::npos) {
							mode |= S_IXUSR;
                		}
                	}

                	if (group.size() > 0)
                    {
                         if (group.find('r',0) != string::npos) {
                             mode |= S_IRGRP;
                         }
                         if (group.find('w',0) != string::npos) {
                             mode |= S_IWGRP;
                         }
                         if (group.find('x',0) != string::npos) {
                             mode |= S_IXGRP;
                         }
                    }

					if (other.size() > 0)
                    {
                        if (other.find('r',0) != string::npos) {
                             mode |= S_IROTH;
                        }
                        if (other.find('w',0) != string::npos) {
                             mode |= S_IWOTH;
                        }
                        if (other.find('x',0) != string::npos) {
                             mode |= S_IXOTH;
                        }
                    }

                	return mode;
                }
            };


        } // namespace Core
    } // namespace Linux
} // namespace ItSoftware
