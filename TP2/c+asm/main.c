#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>

struct MemoryStruct {
    char *memory;
    size_t size;
};

// Write callback for libcurl
static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if(ptr == NULL) return 0;  // out of memory

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

// Extracts "value" fields from JSON
double* extract_gini_values(const char* json_text, int* out_count) {
    cJSON* root = cJSON_Parse(json_text);
    if (!root || !cJSON_IsArray(root)) {
        fprintf(stderr, "Invalid JSON\n");
        return NULL;
    }

    cJSON* data_array = cJSON_GetArrayItem(root, 1);
    if (!cJSON_IsArray(data_array)) {
        fprintf(stderr, "Invalid data array\n");
        cJSON_Delete(root);
        return NULL;
    }

    int count = cJSON_GetArraySize(data_array);
    double* values = malloc(count * sizeof(double));
    int actual_count = 0;

    for (int i = 0; i < count; i++) {
        cJSON* item = cJSON_GetArrayItem(data_array, i);
        if (!item) continue;

        cJSON* value_field = cJSON_GetObjectItem(item, "value");
        if (value_field && cJSON_IsNumber(value_field)) {
            values[actual_count++] = value_field->valuedouble;
        }
    }

    cJSON_Delete(root);
    *out_count = actual_count;
    return values;
}

// Assembly function declaration
extern int suma_1_gini(float value);

int main(void)
{
    CURL *curl;
    CURLcode res;

    struct MemoryStruct chunk;
    chunk.memory = malloc(1);  // will be grown by realloc
    chunk.size = 0;

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.worldbank.org/v2/en/country/AR/indicator/SI.POV.GINI?format=json&date=2011:2022");
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            return 1;
        }

        curl_easy_cleanup(curl);
    }

    int count = 0;
    double* values = extract_gini_values(chunk.memory, &count);
    free(chunk.memory);

    if (!values) return 1;

    printf("Array of values: [");
    for (int i = 0; i < count; i++) {
        printf("%.2f%s", values[i], (i < count - 1) ? ", " : "");
    }
    printf("]\n");

    // Use suma_1_gini to process each value
    printf("\nProcessed values (suma_1_gini):\n");
    for (int i = 0; i < count; i++) {
        int result = suma_1_gini((float)values[i]);  // Assembly function call
        printf("Processed value[%d] = %d\n", i, result);
    }

    free(values);
    curl_global_cleanup();
    return 0;
}
