///////////////////////////////////////////////////////////////////
//: Title        : itsoftware-linux-core.h
//: Product:     : Cpp.Include.Linux
//: Date         : 2020-05-01
//: Author       : "Kjetil Kristoffer Solberg" <post@ikjetil.no>
//: Version      : 1.0.0.0
//: Descriptions : Implementation of Cpp.Include.Linux.
//: Uses         : libuuid.a.
#pragma once
//
// #include
//
#include <string>
#include <cstring>
#include <memory>
#include <algorithm>
#include <thread>
#include <functional>
#include <iostream>
#include <vector>

#include <fcntl.h>
#include <dlfcn.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <uuid/uuid.h>
#include <sys/inotify.h>
#include <signal.h>

#include "itsoftware-linux.h"


//
// namespace
//
namespace ItSoftware::Linux::Core
{
    //
    // using
    //
    using std::make_unique;
    using std::string;
    using std::unique_ptr;
    using std::vector;
    using std::thread;
    using std::function;
    using std::begin;
    using std::end;
    using std::any_of;
    using ItSoftware::Linux::ItsString;
    
    //
    // #define
    //
    #define FILE_MONITOR_BUFFER_LENGTH (10 * (sizeof(inotify_event) + NAME_MAX + 1))

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
            
            return (ts.tv_sec * 1'000'000'000) + ts.tv_nsec;
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
    // struct: ItsGuidFormat
    //
    // (i): Container for premade Guid format strings.
    //
    struct ItsGuidFormat {
        inline static constexpr const char* MicrosoftRegistryFormat{"{%08lX-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X}"};
        inline static constexpr const char* MicrosoftRegistryFormatStripped{ "%08lX-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X" };
        inline static constexpr const char* MicrosoftConstFormat{ "{ 0x%lx, 0x%x, 0x%x, { 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x } }" };
        inline static constexpr const char* MicrosoftCompactFormat{ "%08lX%04X%04x%02X%02X%02X%02X%02X%02X%02X%02X" };
        inline static constexpr const char* MicrosoftPrefixedCompactFormat{ "GUID%08lX%04X%04x%02X%02X%02X%02X%02X%02X%02X%02X" };
    };

    //
    // struct: ItsGuid
    //
    // (i): Wrapper for Guid generation and rendering.
    //
    struct ItsGuid
    {
    public:
        static string CreateGuid()
        {
            uuid_t guid{0};
            uuid_generate(guid);
            return ItsGuid::ToString(guid);
        }
        static bool CreateGuid(uuid_t pGuid)
        {
            uuid_generate_random(pGuid);
            return true;
        }
        static string ToString(uuid_t guid) {
            return ItsGuid::ToString(guid, ItsGuidFormat::MicrosoftRegistryFormat, true);
        }
        static string ToString(uuid_t guid, const string& format, bool isMicrosoftGuidFormat) {
            char szBuffer[100];
            memset(szBuffer, 0, 100);

            if (isMicrosoftGuidFormat) {
                snprintf(szBuffer, 100, format.c_str(),
                    *reinterpret_cast<uint32_t*>(&guid[0]), *reinterpret_cast<uint16_t*>(&guid[4]), *reinterpret_cast<uint16_t*>(&guid[6]),
                    guid[8], guid[9], guid[10], guid[11], guid[12], guid[13], guid[14], guid[15]);
            }
            else {
                snprintf(szBuffer, 100, format.c_str(),
                    guid[0], guid[1], guid[2], guid[3], guid[4], guid[5], guid[6], guid[7],
                    guid[8], guid[9], guid[10], guid[11], guid[12], guid[13], guid[14], guid[15]);
            }
            return string(szBuffer);
        }
    };

    //
    // struct: ItsError
    //
    // (i): Linux error messages
    //
    struct ItsError
    {
    public:
        static string GetErrorDescription(int err) 
        {
            return strerror(err);
        }
        static string GetLastErrorDescription()
        {
            return strerror(errno);
        }
    };
    
    //
    // struct: ItsDirectory
    // 
    // (i): Linux directory routines.
    //
    struct ItsDirectory
    {
    public:
        static bool Exists(const string& dirname)
        {
            struct stat sb;

            if (stat(dirname.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))
            {
                return true;
            }
            return false;
        }

        static string GetCurrentDirectory() 
        {
            char path[PATH_MAX];
            if ( getcwd(path, PATH_MAX) == nullptr ) {
                return string("");
            }
            return string(path);
        }
        static bool CreateDirectory(const string& path, int mode)
        {
            return (mkdir(path.c_str(), mode) == 0);
        }
        static bool RemoveDirectory(const string& path)
        {
            return (rmdir(path.c_str()) == 0);
        }
        static bool SetCurrentDirectory(const string& path)
        {
            return (chdir(path.c_str()) == 0);
        }
        static vector<string> GetDirectories(const string& path) {
            if (path.size() == 0) {
                return vector<string>();
            }

            DIR *dir;
            struct dirent *entry;

            if ((dir = opendir(path.c_str())) == nullptr) {
                return vector<string>();
            }

            vector<string> directories;
            while ((entry = readdir(dir)) != nullptr) {
                if (entry->d_type & DT_DIR) {
                    directories.push_back(entry->d_name);
                }
            }

            closedir(dir);
            
            return directories;
        }
        static vector<string> GetFiles(const string& path) {
            if (path.size() == 0) {
                return vector<string>();
            }

            DIR *dir;
            struct dirent *entry;

            if ((dir = opendir(path.c_str())) == nullptr) {
                return vector<string>();
            }

            vector<string> files;
            while ((entry = readdir(dir)) != nullptr) {
                if (entry->d_type & DT_REG) {
                    files.push_back(entry->d_name);
                }
            }

            closedir(dir);
            
            return files;
        }
    };

    //
    // class: unique_handle
    //
    // (i): Wrapper for many types of Windows handles.
    //
    template<typename Traits>
    class unique_handle
    {
    private:
        typedef typename Traits::pointer pointer;

        pointer m_value;

    protected:
    public:

        explicit unique_handle(pointer value = Traits::invalid()) noexcept
            : m_value{ value }
        {

        }

        ~unique_handle() noexcept
        {
            this->Close();
        }

        auto Close() noexcept -> void
        {
            if (*this)
            {
                Traits::close(this->m_value);
                this->m_value = Traits::invalid();
            }
        }

        pointer p()
        {
            return m_value;
        }

        pointer* GetAddressOf()
        {
            return &m_value;
        }

        //
        // bool
        //
        explicit operator bool() const noexcept
        {
            return m_value != Traits::invalid();
        }

        //
        // unique_so_handle
        //
        operator void*() const noexcept
        {
            return static_cast<void*>(m_value);
        }

        void operator=(void* handle)
        {
            if (this->m_value != Traits::invalid()) {
                this->Close();
            }
            m_value = static_cast<typename Traits::pointer>(handle);
        }

        // 
        // unique_file_handle
        //
        operator int() const noexcept
        {
            return static_cast<int>(m_value);
        }

        void operator=(int handle)
        {
            if (this->m_value != Traits::invalid()) {
                this->Close();
            }
            m_value = static_cast<typename Traits::pointer>(handle);
        }

        //
        // IsInvalid
        //
        bool IsInvalid()
        {
            return (m_value == Traits::invalid());
        }

        //
        // IsValid
        //
        bool IsValid()
        {
            return (m_value != Traits::invalid());
        }
    };

    //
    // shared library descriptor traits
    //
    struct handle_shared_library_traits
    {
        typedef void* pointer;
        static auto invalid() noexcept -> pointer
        {
            return nullptr;
        }

        static auto close(pointer value) noexcept -> void
        {
            ::dlclose(value);
        }
    };

    //
    // shared library descriptor traits
    //
    struct handle_file_traits
    {
        typedef int pointer;
        static auto invalid() noexcept -> pointer
        {
            return -1;
        }

        static auto close(pointer value) noexcept -> void
        {
            ::close(value);
        }
    };

    //
    // typedef unique_handle's.
    //
    typedef unique_handle<handle_shared_library_traits> unique_so_handle;
    typedef unique_handle<handle_file_traits> unique_file_handle;

    //
    // File IO Wrapper
    //
    class ItsFile
    {
        private:
        unique_file_handle m_fd;
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
        bool OpenExisting(const string& filename, const string& flags)
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
        bool OpenOrCreate(const string& filename, const string& flags, int mode)
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
            while (this->Read(reinterpret_cast<void*>(data), 2048, &bytesRead) && bytesRead > 0)
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

        void Close()
        {
            this->m_fd.Close();
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
        static size_t GetFileSize(const string& filename)
        {
            struct stat statbuf;
            if ( stat(filename.c_str(), &statbuf) == -1 ) {
                return -1;
            }
            return statbuf.st_size;
        }

        static bool Delete(const string& filename)
        {
            return (!unlink(filename.c_str()));
        }

        static bool Exists(const string& filename)
        {
            struct stat sb;

            if (stat(filename.c_str(), &sb) == 0 && S_ISREG(sb.st_mode))
            {
                return true;
            }
            
            return false;
        }

        static bool Move(const string& sourceFilename, const string& targetFilename)
        {
            return (!rename(sourceFilename.c_str(), targetFilename.c_str()));
        }

        static bool Copy(const string& sourceFilename, const string& targetFilename, bool replaceIfExists)
        {
            if (!ItsFile::Exists(sourceFilename))
            {
                return false;
            }

            ItsFile source;
            source.OpenExisting(sourceFilename, "r");
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
                ItsFile::GetMode(sourceFilename, &mode);
                target.OpenOrCreate(targetFilename, "wt", mode);
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

        static bool GetMode(const string& filename, int *mode)
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

        static bool SetMode(const string& filename, int mode)
        {
            if (!ItsFile::Exists(filename))
            {
                return false;
            }

            struct stat stat1;
            if (chmod(filename.c_str(), mode) != 0)
            {
                return false;
            }
            return true;
        }

        static int CreateMode(const string& user, const string& group, const string& other)
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

        static bool Shred(const string& filename, bool alsoDelete)
        {
            if (!ItsFile::Exists(filename)) 
            {
                return false;
            }

            const size_t fileSize = ItsFile::GetFileSize(filename);
            if ( fileSize == 0 ) 
            {
                return false;
            }

            ItsFile f;
            f.OpenExisting(filename, "rw");
            if ( f.IsInvalid() ) 
            {
                return false;
            }

            const uint32_t bufferSize = 2048;
            size_t bytesWritten = 0;
            size_t totalWritten = 0;
            const unique_ptr<uint8_t[]> pdata = make_unique<uint8_t[]>(bufferSize);
            for (uint32_t i = 0; i < bufferSize; i++) {
                pdata[i] = 0xFF;
            }
            f.SetPosFromBeg(0);
            while (totalWritten < fileSize) {
                f.Write(static_cast<const void*>(pdata.get()), ((fileSize - totalWritten) > bufferSize) ? bufferSize : (fileSize-totalWritten), &bytesWritten);
                totalWritten += bytesWritten;
            }
            f.Close();

            if (alsoDelete) 
            {
                return ItsFile::Delete(filename);
            }

            return true;
        }

        static bool ShredAndDelete(const string& filename) 
        {
            return ItsFile::Shred(filename,true);
        }

        static bool ReadAllText(const string& filename, string& textRead) 
        {
            if (!ItsFile::Exists(filename)) {
                return false;
            }

            ItsFile file{};
            string flags("rw");
            if (!file.OpenExisting(filename, flags)) {
                return false;
            }

            file.SetPosFromBeg(0);
            if (!file.ReadAllText(textRead)) {
                return false;
            }

            file.Close();
            return true;
        }

        static bool ReadTextAllLines(const string& filename, vector<string>& textLines) 
        {
            if (!ItsFile::Exists(filename)) {
                return false;
            }

            ItsFile file{};
            string flags("rw");
            if (!file.OpenExisting(filename, flags)) {
                return false;
            }

            file.SetPosFromBeg(0);
            string textRead;
            if (!file.ReadAllText(textRead)) {
                return false;
            }

            file.Close();

            textLines.clear();
            textLines = ItsString::Split(textRead, "\n");
            
            return true;
        }
    };

    //
    // struct: ItsPath
    // 
    // (i): Path routines.
    //
    struct ItsPath
    {
    public:
        static const char PathSeparator = '/';
        static const char ExtensionSeparator = '.';
        
        static const vector<char> GetInvalidPathCharacters()
        {
            vector<char> chars {'\0'};
            return chars;
        }
        static const vector<char> GetInvalidFilenameCharacters()
        {
            vector<char> chars = ItsPath::GetInvalidPathCharacters();
            chars.push_back('/');
            return chars;
        }
        static string Combine(const string& path1, const string& path2)
        {
            if (path1.size() == 0 && path2.size() == 0) {
                return string("");
            }

            if (path1.size() == 0) {
                return path2;
            }

            if (path2.size() == 0) {
                return path1;
            }

            stringstream path;
            path << path1;
            if (path1[path1.size() - 1] != ItsPath::PathSeparator &&
                path2[0] != ItsPath::PathSeparator ) {
                path << ItsPath::PathSeparator;
            }
            path << path2;
            
            string retVal = path.str();
            return retVal;
        }
        static bool Exists(const string& path)
        {
            if (access(path.c_str(), F_OK) == 0) {
                return true;
            }
            return false;
        }
        static bool IsFile(const string& path)
        {
            return ItsFile::Exists(path);
        }
        static bool IsDirectory(const string& path)
        {
            return ItsDirectory::Exists(path);
        }
        static string GetDirectory(const string& path) 
        {
            if (path.size() == 0) {
                return string("");
            }

            if (path.find(ItsPath::PathSeparator) == string::npos) {
                return string("");
            }

            auto i = path.find_last_of(ItsPath::PathSeparator);
            if (i == std::string::npos) {
                return string("");
            }
            return path.substr(0, i+1);
        }
        static string GetFilename(const string& path) 
        {
            if (path.size() == 0) {
                return string("");
            }
            
            if (path.find(ItsPath::PathSeparator) == string::npos) {
                return string("");
            }

            auto i = path.rfind(ItsPath::PathSeparator);
            if (i == std::string::npos) {
                return string("");
            }
            return path.substr(i+1, path.size()-i-1);
        }
        static string GetExtension(const string& path)
        {
            if (path.size() == 0) {
                return string("");
            }

            if (path.find(ItsPath::PathSeparator) == string::npos) {
                return string("");
            }

            auto i = path.rfind(ItsPath::ExtensionSeparator);
            if (i == std::wstring::npos) {
                return string("");
            }
            return path.substr(i, path.size() - i);
        }
        static bool IsPathValid(const string& path)
        {
            if (path.size() == 0) {
                return false;
            }

            string directory = ItsPath::GetDirectory(path);
            string filename = ItsPath::GetFilename(path);
            
            auto invalidPathChars = ItsPath::GetInvalidPathCharacters();
            auto invalidFileChars = ItsPath::GetInvalidFilenameCharacters();
                                
            if ( directory[directory.size()-1] != ItsPath::PathSeparator ) {
                return false;
            }

            return std::all_of(directory.begin(), directory.end(), [&](wchar_t d) {
                return std::none_of(invalidPathChars.begin(), invalidPathChars.end(), [&](wchar_t i) {
                    return d == i;
                    });
                });

            return std::all_of(filename.begin(), filename.end(), [&](wchar_t d) {
                return std::none_of(invalidFileChars.begin(), invalidFileChars.end(), [&](wchar_t i) {
                    return d == i;
                    });
                });

            return true;
        }
        static bool HasExtension(const string& path, const string& extension)
        {
            if (path.size() == 0) {
                return false;
            }

            string ext = ItsPath::GetExtension(path);
            if (ext.size() == 0) {
                return false;
            }

            return (strcmp(ext.c_str(), extension.c_str()) == 0);
        }
        static string ChangeExtension(const string& path, const string& newExtension) 
        {
            if (path.size() == 0) {
                return string("");
            }

            if (newExtension.size() == 0) {
                return path;
            }

            if (path[path.size() - 1] == ItsPath::PathSeparator) {
                return path;
            }

            string ext = ItsPath::GetExtension(path);
            if (ext.size() == 0) {
                if (newExtension[0] == ItsPath::ExtensionSeparator) {
                    return path + newExtension;
                }
                else {
                    return path + ItsPath::ExtensionSeparator + newExtension;
                }
            }

            auto pe = path.rfind(ext);
            if (pe == string::npos) {
                return path;
            }

            string path_str = path;
            string retVal = path_str.replace(pe, path.size()-pe, newExtension);
            return retVal;
        }

        static string GetParentDirectory(const string& path) 
        {
            if (path.size() == 0) {
                return string("");
            }

            auto path_str = ItsPath::GetDirectory(path);
            
            size_t pos1 = path_str.rfind(ItsPath::PathSeparator);
            if (pos1 == string::npos) {
                return string("");
            }

            size_t pos2 = pos1;
            if (pos1 == (path_str.size() - 1)) {
                pos2 = path_str.rfind(ItsPath::PathSeparator, pos1 - 1);
                if (pos2 == string::npos) {
                    pos2 = pos1;
                }
            }

            return path_str.substr(0, pos2+1);
        }

    };

    //
    // enum: ItsFileMonitorMask
    //
    // (i): mask for ItsFileMonitor
    //
    enum ItsFileMonitorMask : uint32_t {
        Access = IN_ACCESS,
        Attrib = IN_ATTRIB,
        CloseWrite = IN_CLOSE_WRITE,
        CloseNoWrite = IN_CLOSE_NOWRITE,
        Create = IN_CREATE,
        Delete = IN_DELETE,
        DeleteSelf = IN_DELETE_SELF,
        Modify = IN_MODIFY,
        MoveSelf = IN_MOVE_SELF,
        MovedFrom = IN_MOVED_FROM,
        MovedTo = IN_MOVED_TO,
        Open = IN_OPEN
    };

    //
    // class: ItsFileMonitor
    //
    // (i): Monitors a given file folder
    //
    class ItsFileMonitor
    {
    private:
        int m_fd;
        int m_wd;
        int m_errno;
        uint32_t m_mask;
        thread m_thread;
        string m_pathname;
        bool m_bPaused;
        bool m_bStopped;
        bool m_bInitWithError;
        
    protected:
        void ExecuteDispatchThread(function<void(inotify_event&)> func) {
            char buffer[FILE_MONITOR_BUFFER_LENGTH];
            while ( !this->m_bStopped ) {
                if (this->m_bPaused) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                    continue;
                }
                
                ssize_t nRead = read(this->m_fd, buffer, FILE_MONITOR_BUFFER_LENGTH);
                if (nRead == 0) {
                    continue;
                }
                if (nRead == -1) {
                    continue;
                }
                for (char *p = buffer; p < buffer + nRead;) {
                    inotify_event* event = reinterpret_cast<inotify_event*>(p);
                    func(*event);
                    p += sizeof(inotify_event) + event->len;
                }
            }
        }
    public:
        ItsFileMonitor(const string& pathname, function<void(inotify_event&)> func)
            :   ItsFileMonitor(pathname, (ItsFileMonitorMask::Modify|ItsFileMonitorMask::Open), func) 
        {
            
        }
        ItsFileMonitor(const string& pathname, uint32_t mask, function<void(inotify_event&)> func)
            :   m_pathname(pathname), 
                m_mask(mask),
                m_errno(0),
                m_bInitWithError(true),
                m_fd(-1),
                m_wd(-1),
                m_bPaused(false),
                m_bStopped(false)
        {
            if (ItsDirectory::Exists(this->m_pathname) ) {
                this->m_fd = inotify_init();
                if (this->m_fd == -1) {
                    this->m_errno = errno;
                    return;
                }
                fcntl(this->m_fd, F_SETFL, fcntl(this->m_fd, F_GETFL) | O_NONBLOCK);

                this->m_wd = inotify_add_watch(this->m_fd, pathname.c_str(), mask);
                if (this->m_wd == -1) {
                    this->m_errno = errno;
                    return;
                }
            
                m_thread = thread(&ItsFileMonitor::ExecuteDispatchThread, this, func);
                this->m_bInitWithError = false;
            }
            else {
                this->m_errno = ENOENT;
            }
        }
        bool GetInitWithError()
        {
            return this->m_bInitWithError;
        }
        int GetInitWithErrorErrno()
        {
            return this->m_errno;
        }
        void Pause() {
            this->m_bPaused = true;
        }
        void Resume() {
            this->m_bPaused = false;
        }
        bool IsPaused() {
            return this->m_bPaused;
        }
        void Stop() {
            this->m_bStopped = true;
        }
        bool IsStopped()
        {
            return this->m_bStopped;
        }
        ~ItsFileMonitor()
        {
            this->Stop();
            
            if ( this->m_thread.joinable()) {
                this->m_thread.join();
            }

            if ( this->m_fd != -1 && this->m_wd != -1) {
                int result = inotify_rm_watch(this->m_fd,this->m_wd);
                if (result == 0) {
                    this->m_wd = -1;

                    close(this->m_fd);
                    this->m_fd = -1;
                }
            }
        }
    };

    //
    // class: ItsDaemon
    //
    // (i): Linux daemon plugin
    //
    class ItsDaemon
    {
    private:
        int m_deamonRetVal;
        struct sigaction m_saT;
        struct sigaction m_saC;
        struct sigaction m_saH;
        inline static bool s_sigterm = false;
        inline static bool s_sighup = false;
        inline static function<void(void)> s_fnSigTerm;
        inline static function<void(void)> s_fnSigCont;
        inline static function<void(void)> s_fnSigHup;
    protected:
        static void SigHandler(int sig) {
            switch(sig)
            {                
                case SIGTERM:
                    ItsDaemon::s_sigterm = true;
                    if (ItsDaemon::s_fnSigTerm != nullptr) {
                        ItsDaemon::s_fnSigTerm();
                    }
                    break;
                case SIGCONT:                    
                    if (ItsDaemon::s_fnSigCont != nullptr) {
                        ItsDaemon::s_fnSigCont();
                    }
                    break;
                case SIGHUP:
                    ItsDaemon::s_sighup = true;
                    if (ItsDaemon::s_fnSigHup != nullptr) {
                        ItsDaemon::s_fnSigHup();
                    }
                    break;
                default:
                    break;
            }
        }

        int BecomeDaemon(int flags)
        {
            int fd;

            switch(fork()) {
                case -1: return -1;
                case 0: break;
                default: _exit(EXIT_SUCCESS);
            }

            if (setsid() == -1) {
                return -1;
            }

            switch(fork()) {
                case -1: return -1;
                case 0: break;
                default: _exit(EXIT_SUCCESS);
            }

            if (!(flags & 010)) {
                umask(0);
            }

            if (!(flags & 01)) {
                chdir("/");
            }

            if (!(flags & 02)) {
                int maxfd = sysconf(_SC_OPEN_MAX);
                if (maxfd == -1) {
                    maxfd = 8192;
                }
                for (fd = 0; fd < maxfd; fd++) {
                    close(fd);
                }
            }

            if (!(flags & 04)) {
                close(STDIN_FILENO);

                fd = open("/dev/null", O_RDWR);

                if ( fd != STDIN_FILENO) {
                    return -1;
                }
                if (dup2(STDIN_FILENO, STDOUT_FILENO) != STDOUT_FILENO) {
                    return -1;
                }
                if (dup2(STDIN_FILENO, STDERR_FILENO) != STDERR_FILENO) {
                    return -1;
                }
            }

            return 0;
        }

    public:            
        //
        // Method: Constructor.
        //
        // (i): Constructor.
        //
        explicit ItsDaemon()
        : ItsDaemon(0) {

        }
        //
        // Method: Constructor
        //
        // (i): Constructor.
        //
        explicit ItsDaemon(int flags) {            
            this->m_deamonRetVal = this->BecomeDaemon(flags);
            if ( this->m_deamonRetVal == 0 ) {
                // SIGTERM
                sigemptyset(&this->m_saT.sa_mask);
                this->m_saT.sa_flags = SA_RESTART;
                this->m_saT.sa_handler = ItsDaemon::SigHandler;
                if (sigaction(SIGTERM,&this->m_saT, NULL) == -1 ) {
                    _exit(-1);
                }
                // SIGCONT
                sigemptyset(&this->m_saC.sa_mask);
                this->m_saC.sa_flags = SA_RESTART;
                this->m_saC.sa_handler = ItsDaemon::SigHandler;
                if (sigaction(SIGCONT,&this->m_saC, NULL) == -1 ) {
                    _exit(-1);
                }
                // SIGHUP
                sigemptyset(&this->m_saH.sa_mask);
                this->m_saH.sa_flags = SA_RESTART;
                this->m_saH.sa_handler = ItsDaemon::SigHandler;
                if (sigaction(SIGHUP,&this->m_saH, NULL) == -1 ) {
                    _exit(-1);
                }
            }
        }
        //
        // Method: WasDaemonSuccessful
        //
        // (i): Returnes true is daemon was correctly set up. false otherwise.
        //
        bool GetInitWithError() {
            return (this->m_deamonRetVal != 0);
        }
        //
        // Function: GetSigTerm
        //
        // (i): normal daemon termination signal. should shut down daemon.
        //
        static bool GetSigTerm() {
            return ItsDaemon::s_sigterm;
        }        
        //
        // Function: GetSigHup
        //
        // (i): normal reload of configuration data signal for daemons.
        //
        static bool GetSigHup() {
            return ItsDaemon::s_sighup;
        }
        //
        // Function: ClearSignals
        //
        // (i): Resets signal flags to not set.
        //
        static void ClearSignalFlags()
        {
            s_sigterm = false;
            s_sighup = false;
        }
        //
        // Function: SetSigKill
        //
        // (i): set handler for SIGKILL
        //
        static void SetSigTerm(function<void(void)> fn)
        {
            ItsDaemon::s_fnSigTerm = fn;
        }
        //
        // Function: SetSigKill
        //
        // (i): set handler for SIGCONT
        //
        static void SetSigCont(function<void(void)> fn)
        {
            ItsDaemon::s_fnSigCont = fn;
        }
        //
        // Function: SetSigHup
        //
        // (i): set handler for SIGHUP
        //
        static void SetSigHup(function<void(void)> fn)
        {
            ItsDaemon::s_fnSigHup = fn;
        }
    };
} // namespace ItSoftware::Linux::Core
