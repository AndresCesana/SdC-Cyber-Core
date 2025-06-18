#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>

struct MemoryStruct {
    char *memory;
    size_t size;
};

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if(ptr == NULL) return 0;

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

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

int main(void)
{
    CURL *curl;
    CURLcode res;

    struct MemoryStruct chunk;
    chunk.memory = malloc(1);
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

    printf("Float array from API (GINI index):[");
    for (int i = 0; i < count; i++) {
        printf("%.2f%s", values[i], (i < count - 1) ? ", " : "");
    }
    printf("]");

    printf("\nFloat values incremented by 1: [");
    for (int i = 0; i < count; i++) {
        values[i] += 1.0;
        printf("%.2f%s", values[i], (i < count - 1) ? ", " : "");
    }
    printf("]\n");

    int* int_values = malloc(count * sizeof(int));
    if (!int_values) {
        fprintf(stderr, "Memory allocation failed for int_values\n");
        free(values);
        return 1;
    }

    printf("\nInteger-converted values: [");
    for (int i = 0; i < count; i++) {
        int_values[i] = (int)values[i];
        printf("%d%s", int_values[i], (i < count - 1) ? ", " : "");
    }
    printf("]");

    printf("\nInteger values incremented by 1: [");
    for (int i = 0; i < count; i++) {
        int_values[i] += 1;
        printf("%d%s", int_values[i], (i < count - 1) ? ", " : "");
    }
    printf("]\n");

    free(int_values);
    free(values);
    return 0;
}
