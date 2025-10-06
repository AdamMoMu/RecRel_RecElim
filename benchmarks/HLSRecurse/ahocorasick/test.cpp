
#include "header.hpp"
#include <iostream>

using namespace std;

int main() {
    int substring_count = 3;
    string substrings = "abababa%aaaa%aaaababababababababababababaaaaa%";
    string query = "aaaababababababababababababaaaaaaaaa%";

    int substrings_size = substrings.size();

    // Allocate and fill a char array for substrings_buffer
    char* substrings_buffer = new char[substrings.size()];
    for (size_t i = 0; i < substrings.size(); i++) {
        substrings_buffer[i] = substrings[i];
    }

    // Allocate and fill a char array for query_buffer
    char* query_buffer = new char[query.size()];
    for (size_t i = 0; i < query.size(); i++) {
        query_buffer[i] = query[i];
    }

    int worst_case_matches = query.size() * substring_count;

    // Allocate integer arrays for substring_indexes and query_indexes
    int* substring_indexes = new int[worst_case_matches];
    int* query_indexes = new int[worst_case_matches];

	int fallback = 0;

    // Call process_top using the allocated arrays
    process_top(&substrings_size, substrings_buffer, query_buffer,
                substring_indexes, query_indexes, &fallback);

    // Process and output results
    for (int i = 0; ; i++) {
        if (substring_indexes[i] == -1)
            break;
        for (int j = substring_indexes[i]; substrings[j] != '%'; j++) {
            cout << substrings[j];
        }
        cout << ":" << query_indexes[i] << endl;
    }

    // Clean up allocated memory
    delete[] substrings_buffer;
    delete[] query_buffer;
    delete[] substring_indexes;
    delete[] query_indexes;

    return 0;
}

// Expected Output:
// aaaa:3
// abababa:9
// abababa:11
// abababa:13
// abababa:15
// abababa:17
// abababa:19
// abababa:21
// abababa:23
// abababa:25
// abababa:27
// aaaa:30
// aaaababababababababababababaaaaa:31
// aaaa:31
// aaaa:32
// aaaa:33
// aaaa:34
// aaaa:35