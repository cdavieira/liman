#pragma

void process_throw(int code, const char *msg);

void process_abort(const char *msg);

void process_quick_abort(void);

void process_abort_at(const char *filename, int line);
#define todo() process_abort_at(__FILE_NAME__, __LINE__)
