#ifdef OPENGLTASK_EXPORTS
#define OPENGLTASK_API __declspec(dllexport)
#else
#define OPENGLTASK_API __declspec(dllimport)
#endif