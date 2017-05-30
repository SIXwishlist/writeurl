#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

#include <writeurl/file.hpp>
#include <writeurl/error.hpp>

using namespace writeurl;

bool file::file_exists(const std::string& path)
{
    struct stat buffer;   
    return (stat(path.c_str(), &buffer) == 0); 
}

std::string file::read_file(const std::string& path, std::error_code& ec)
{
    int fd = open(path.c_str(), O_RDONLY);
    if (fd == -1) {
        if (errno == ENOENT) {
            ec = make_error_code(Error::file_no_exist);
        }
        else if (errno == EACCES) {
            ec = make_error_code(Error::file_read_access_denied);
        }
        else {
            ec = make_error_code(Error::file_unspecified_error);
        }

        return "";
    }

    struct stat buffer;   
    int rc = fstat(fd, &buffer);
    if (rc == -1) {
        ec = make_error_code(Error::file_unspecified_error);
        return "";
    }

    size_t file_size = size_t(buffer.st_size);

    std::string result;
    result.reserve(file_size);

    int nread = read(fd, (void*) result.data(), file_size);
    if (nread != file_size) {
        ec = make_error_code(Error::file_unspecified_error);
        return "";
    }

    ec = std::error_code{};
    return result;
}

void file::write_file(const std::string& path, std::error_code& ec)
{

}
