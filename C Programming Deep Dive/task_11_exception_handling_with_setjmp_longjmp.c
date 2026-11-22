#include <stdio.h>
#include <setjmp.h>
#include <stdlib.h>
#include <string.h>

/* Incomplete starter — implement the required behavior.
 * Stdin modes (if any): file, calc, success, nested
 * Match the sample test output exactly. Hidden tests use the other inputs listed above.
 */

/* Core types from the exercise (keep these) */
typedef enum {
    NO_ERROR = 0,
    FILE_ERROR = 1,
    CALCULATION_ERROR = 2,
    NESTED_ERROR = 3
} Exception;

typedef struct JmpBufInfo
{
    int error;
    jmp_buf* env;
} JmpBufInfo;

int top = -1;
JmpBufInfo stack[64];

#define TRY jmp_buf env; JmpBufInfo jup_buf_info; jup_buf_info.env = &env; jup_buf_info.error = setjmp(env); add_info(jup_buf_info); if (jup_buf_info.error == 0)
#define CATCH(exception) else if (get_top_info().error == (exception))
#define FINALLY if(1)
#define ETRY pop_info();
#define THROW(exception) longjmp(*get_top_info().env, (exception));

JmpBufInfo get_top_info()
{
    return stack[top];
}

void add_info(JmpBufInfo info)
{
    stack[++top] = info;
}

void pop_info()
{
    if (top >= 0)
        --top;
}

/* TODO: implement read_file */
void read_file(char* name)
{
    if (name == NULL || strcmp(name, "") == 0)
    {
        THROW(FILE_ERROR);
    }
}

/* TODO: implement calculate */
void calculate(int value)
{
    if (value == 0)
    {
        THROW(CALCULATION_ERROR);
    }

    run_success(100 / value);
}

/* TODO: implement nested_demo */
void nested_demo()
{
    jmp_buf jb;
    setjmp(jb);
    longjmp(jb, NESTED_ERROR);
}

/* TODO: implement run_file_error */
void run_file_error()
{
    printf("Caught file error\n");
}

/* TODO: implement run_calc_error */
void run_calc_error()
{
    printf("Caught calculation error\n");
}

/* TODO: implement run_success */
void run_success(int result)
{
    printf("Success! Result: %d\n", result);
}

int main(void) {
    char mode[64] = "";
    if (scanf("%63s", mode) != 1) {
        mode[0] = '\0';
    }

    int is_all = strcmp(mode, "file") != 0 && strcmp(mode, "calc") != 0 && strcmp(mode, "success") != 0 && strcmp(mode, "nested") != 0;

    if (is_all || strcmp(mode, "file") == 0)
    {
        TRY
        {
            read_file("");
        }
        CATCH (FILE_ERROR)
        {
            run_file_error();
        }
        FINALLY
        {
            printf("Cleanup code\n");
        }
        ETRY
    }
    if (is_all || strcmp(mode, "calc") == 0)
    {
        TRY
        {
            calculate(0);
        }
        CATCH (CALCULATION_ERROR)
        {
            run_calc_error();
        }
        FINALLY
        {
            printf("Cleanup code\n");
        }
        ETRY
    }
    if (is_all || strcmp(mode, "success") == 0)
    {
        TRY
        {
            calculate(10);
        }
        CATCH (CALCULATION_ERROR)
        {
            run_calc_error();
        }
        FINALLY
        {
            printf("Cleanup code\n");
        }
        ETRY
    }
    if (is_all || strcmp(mode, "nested") == 0)
    {
        TRY
        {
            TRY 
            {
                THROW(NESTED_ERROR)
            }
            CATCH (NESTED_ERROR)
            {
                printf("Nested exception handled\n");
            }
            ETRY
        }
        ETRY
    }

    return 0;
}