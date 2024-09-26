#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

struct MemoryStruct {
    char *memory;
    size_t size;
};

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);

    if (ptr == NULL) {
        printf("Not enough memory (realloc returned NULL)\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

void download_image(const char *url, const char *filename) {
    CURL *curl_handle;
    FILE *fp;

    curl_handle = curl_easy_init();
    if (curl_handle) {
        fp = fopen(filename, "wb");
        if (!fp) {
            fprintf(stderr, "Failed to open file %s for writing\n", filename);
            return;
        }

        curl_easy_setopt(curl_handle, CURLOPT_URL, url);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, fp);
        curl_easy_setopt(curl_handle, CURLOPT_FAILONERROR, 1L);

        CURLcode res = curl_easy_perform(curl_handle);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        } else {
            printf("Image successfully downloaded to %s\n", filename);
        }

        fclose(fp);
        curl_easy_cleanup(curl_handle);
    }
}

int main(void) {
    struct MemoryStruct chunk;
    chunk.memory = malloc(1);
    chunk.size = 0;

    CURL *curl_handle;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_ALL);

    curl_handle = curl_easy_init();

    if(curl_handle) {
        // oh no, I got all the wallpapers :(
        curl_easy_setopt(curl_handle, CURLOPT_URL, "https://storage.googleapis.com/panels-api/data/20240916/media-1a-i-p~s");
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
        res = curl_easy_perform(curl_handle);

        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        } else {
            printf("\n\n\n-> %lu bytes retrieved\n\n\n", (unsigned long)chunk.size);
            char *dhd_ptr = chunk.memory;
            int image_counter = 1;
            
            while ((dhd_ptr = strstr(dhd_ptr, "\"dhd\"")) != NULL) {
                dhd_ptr = strchr(dhd_ptr, ':');
                if (dhd_ptr) {
                    dhd_ptr++;
                    while (*dhd_ptr == ' ' || *dhd_ptr == '\"') dhd_ptr++;

                    char *end_ptr = strchr(dhd_ptr, '\"');
                    if (end_ptr) {
                        size_t url_len = end_ptr - dhd_ptr;
                        char *dhd_url = malloc(url_len + 1);
                        strncpy(dhd_url, dhd_ptr, url_len);
                        dhd_url[url_len] = '\0';

                        char filename[50];
                        snprintf(filename, sizeof(filename), "dhd_image_%d.jpg", image_counter);

                        printf("Found 'dhd' URL: %s\n", dhd_url);
                        download_image(dhd_url, filename);

                        free(dhd_url);
                        image_counter++;
                    }
                }
            }
        }

        curl_easy_cleanup(curl_handle);
        free(chunk.memory);
    }

    curl_global_cleanup();

    return 0;
}