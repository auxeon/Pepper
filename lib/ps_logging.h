#ifndef PS_LOGGING_H
#define PS_LOGGING_H

#define ERROR(...) printf("[E] ");printf(__VA_ARGS__); printf("\n");
#define WARN(...) printf("[W] ");printf(__VA_ARGS__); printf("\n");
#define INFO(...) printf("[I] ");printf(__VA_ARGS__); printf("\n");

#endif