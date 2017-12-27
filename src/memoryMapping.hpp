#include <iostream>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>
#include <time.h>
#include <cstdint>  // int_least32_t

#define NUMINTS (16)
#define FILE_LENGTH (NUMINTS * 1)

class MemoryMapping{
    char* filename;

public:
    MemoryMapping(char* fname) : filename(fname){}

    int open(){
        int fd;
        fd = ::open(filename, O_RDONLY);
        if (fd == -1) {
            perror("Error opening file for reading");
            exit(EXIT_FAILURE);
        }
        return fd;
    }

    char *read_next(int fd){
        char *map;
        map = (char *)mmap(0, FILE_LENGTH, PROT_READ, MAP_SHARED, fd, 0);
        if (map == MAP_FAILED) {
            ::close(fd);
            perror("Error mmapping the file");
            exit(EXIT_FAILURE);
        }
        return map;
    }

    // TODO MISSING ENDOFSTREAM

    void unmap(char *map){
        if (munmap(map, FILE_LENGTH) == -1) {
            perror("Error un-mmapping the file");
        }
    }

    void printMapElements(char *map){
        /* Read the file int-by-int from the map
        */
        for (int i = 0; i <NUMINTS; ++i) {
            std::cout << i << ":" << map[i] << "  ";
        }
        std::cout << std::endl;
    }

    char *read_file(){
        int i;
        int fd;
        char *map;  /* mmapped array of int's */
        // Open the file.
        fd = open();
        // Read the information.
        map = read_next(fd);
        // print for testing.
        printMapElements(map);
        // Unmap
        unmap(map);
        // Close because unmap doesn't ::close it.
        ::close(fd);

        return map;
    }

    // TODO
    // TODO WE MAY CHANGE THE SIGNATURE TO GIVE HIM DIRECTLY A LIST OF int_least32_t
    // TODO

    char *write(){
        //srand(time(NULL));
        int i;
        int fd;
        int result;
        char *map;  /* mmapped array of int's */

        /* Open a file for writing.
         *  - Creating the file if it doesn't exist.
         *  - Truncating it to 0 size if it already exists. (not really needed)
         */
        fd = ::open(filename, O_RDWR | O_CREAT | O_TRUNC, (mode_t)0600);
        if (fd == -1) {
            perror("Error opening file for writing");
            exit(EXIT_FAILURE);
        }

        /* Stretch the file size to the size of the (mmapped) array of ints
        */
        result = lseek(fd, FILE_LENGTH-1, SEEK_SET);
        if (result == -1) {
            ::close(fd);
            perror("Error calling lseek() to 'stretch' the file");
            exit(EXIT_FAILURE);
        }

        /* Something needs to be written at the end of the file to
         * have the file actually have the new size.
         * Just writing an empty string at the current file position will do.
         */
        result = ::write(fd, "", 1);
        if (result != 1) {
            ::close(fd);
            perror("Error writing last byte of the file");
            exit(EXIT_FAILURE);
        }

        map = (char *)mmap(0, FILE_LENGTH, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if (map == MAP_FAILED) {
            ::close(fd);
            perror("Error mmapping the file");
            exit(EXIT_FAILURE);
        }

        // TODO
        // TODO HERE WE CAN ADD A LIST OF INT32, THAT WE WILL CONVERT INTO CHAR AND WRITE IN FILE int_least32_t
        // TODO
        for (i = 0; i <NUMINTS; ++i) {
            map[i] = 'a';
        }

        /* Don't forget to free the mmapped memory
        */
        if (munmap(map, FILE_LENGTH) == -1) {
            perror("Error un-mmapping the file");
            /* Decide here whether to ::close(fd) and exit() or not. Depends... */
        }

        /* Un-mmaping doesn't ::close the file, so we still need to ::close it.
        */
        ::close(fd);
        return map;
    }
};

// TODO: split into hpp/cpp

// vim: set shiftwidth=4 softtabstop=4 spell spelllang=en:
