#ifndef CONFIG_URL_H
#define CONFIG_URL_H

/**
 * @file
 * @brief functions used for manipulate URLs
 */

/**
 * rewrite_url - check if the URL is equals to "/".
 *
 * @param url - The URL needs to be analyzed.
 *
 * @return The rewrite_url function return "/index.html" if the URL parameter
 * passed in is the same as "/", if the passed URL contains '?' the function
 * will return all the character before the '?' else the funciton returns the
 * same URL as that given in parameter.
 */
const char  *rewrite_url(char *url);

/**
 * check_and_open - check if file given by url exsit and open it.
 *
 * @param url - The path of the file request.
 * @param document_root - The path which the project is located.
 *
 * @return On succes chek_and_open return the new file descriptor, or 1 if an
 * error occurred.
 */
int check_and_open(const char *url, const char *document_root);

/**
 * get_file_size - gives the size of the file in bytes.
 *
 * @param fildes - The file descriptor of the file whose size is desired.
 *
 * @return On success, the size in bytes is returned. On error, 1 is returned.
 */
int get_file_size(int fildes);

/**
 * copy - read into file descriptor in and rewrite into out.
 *
 * @param in - The in file descriptor.
 * @param out - The out file descriptor.
 *
 * @return On success, the number of bytes written is returned. On error, -1 is
 * returned.
 */
int copy(int in, int out);

/**
 * application_types - gives the type mines of file.
 *
 * @param url - The path to file.
 *
 * @return The type mines.
 */
const char *application_type(const char *url);

/**
 * url_valid - avoid url contains "..".
 *
 * @param url - The URL to check up.
 *
 * @return The url_valid function return 1 if URL does't contains "..",
 * Otherwise 0 is returned.
 */
int url_valid(char *url);

#endif
