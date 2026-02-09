/*
 Author: Amogh Peravali
 netID: asp301
 RUID: 214006911

 Below is the code for the group 1 assignment part 4

*/


// Allows use of std::cout and std::cerr
#include <iostream>


// Allows use of std::ofstream for writing to files
#include <fstream>


// Needed for open() and file flags
#include <fcntl.h>

// Include UNIX standard functions
// Needed for write(), close(), etc.
#include <unistd.h>

// Include C-style string functions
// Needed for memset()
#include <cstring>

// Include file system statistics
// Needed for statvfs() to get block size
#include <sys/statvfs.h>

// Main function: program execution starts here
int main()
{


// This is part of the code for 
   

    // Print a message to standard output indicating Part i is starting
    std::cout << "PART i: Writing to file using ofstream...\n";

    // Create an output file stream object
    // This opens (or creates) the file "part1.txt"
    std::ofstream file("part1.txt");

    // Check if the file opened successfully
    if (!file)
    {
        // If file opening failed, print error to standard error
        std::cerr << "Error opening part1.txt\n";

        // Exit program with error code
        return 1;
    }

    // Write text into the file using the output file stream
    file << "This was written using C++ ofstream.\n";

    // Close the file to save changes and release resources
    file.close();

  
 // This is the part where it shows stdout, stderr, and redirection
    

    // Print message indicating Part ii is starting
    std::cout << "\nPART ii: Writing to stdout and stderr...\n";

    // Write message to standard output stream (stdout)
    std::cout << "This is standard output (cout)\n";

    // Write message to standard error stream (stderr)
    std::cerr << "This is standard error (cerr)\n";

   
   // This is where there is a modification of low-performance level 2 I/O 


    // Print message indicating Part iii is starting
    std::cout << "\nPART iii: Low-level buffered file writing...\n";

    // Open (or create) a file using low-level system call
    // O_WRONLY  -> open for writing
    // O_CREAT   -> create file if it doesn't exist
    // O_TRUNC   -> erase file contents if it already exists
    // 0644      -> file permissions (rw-r--r--)
    int fd = open("part3.txt",
                  O_WRONLY | O_CREAT | O_TRUNC,
                  0644);

    // Check if file opened successfully
    if (fd < 0)
    {
        // Print system-generated error message
        perror("open");

        // Exit program with error code
        return 1;
    }

    // Define buffer size as 32 kilobytes (32 * 1024 bytes)
    // Power of 2 size improves performance
    const size_t BUFFER_SIZE = 32 * 1024;

    // Create a character buffer array of BUFFER_SIZE bytes
    char buffer[BUFFER_SIZE];

    // Fill entire buffer with the character 'A'
    // This prepares data for writing
    memset(buffer, 'A', BUFFER_SIZE);

    // Loop 100 times to write large chunks of data
    // This reduces the number of system calls
    for (int i = 0; i < 100; i++)
    {

        // Write the buffer contents to the file
        // Returns number of bytes written
        ssize_t bytes = write(fd, buffer, BUFFER_SIZE);

        // Check if write failed
        if (bytes < 0)
        {
            // Print system error message
            perror("write");

            // Close file descriptor
            close(fd);

            // Exit program with error code
            return 1;
        }
    }

    // Close the file descriptor after writing is complete
    close(fd);

    
// Finding the block size of the drive 

    // Print message indicating Part iv is starting
    std::cout << "\nPART iv: Finding disk block size...\n";

    // Create structure to hold filesystem information
    struct statvfs info;

    // Get file system statistics for current directory
    if (statvfs(".", &info) == 0)
    {
        // Print the block size in bytes
        std::cout << "Disk block size: "
                  << info.f_bsize
                  << " bytes\n";

        // Suggest an optimal buffer size (8 blocks)
        std::cout << "Recommended buffer size: "
                  << info.f_bsize * 8
                  << " bytes\n";
    }
    else
    {
        // Print error message if statvfs fails
        perror("statvfs");
    }

    // Print completion message
    std::cout << "\nAll parts completed successfully.\n";

    // Return 0 to indicate successful program execution
    return 0;
}
