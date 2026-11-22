#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* Incomplete starter — implement the required behavior.
 * Stdin modes (if any): alloc, reset, align, oom
 * The brief lists every input mode and the exact output.
 */

/* Core types from the exercise (keep these) */
#define ARENA_SIZE 1024
typedef struct Arena {
    char* buffer;
    size_t size;
    size_t offset;
} Arena;

/* TODO: implement arena_create */
Arena* arena_size(int size)
{
    Arena* arena = (Arena*) malloc(sizeof(Arena));
    arena->buffer = malloc(sizeof(char) * size);
    arena->size = size;
    arena->offset = 0;
    return arena;
}

/* TODO: implement arena_alloc */
void* arena_alloc(Arena* arena, int size, int alignment)
{
    int offset = (arena->offset + alignment - 1) & ~(alignment - 1);

    if (offset + size > arena->size)
        return NULL;

    void* alloc = (void*) (arena->buffer + offset);
    arena->offset = offset + size;
    return alloc;
}

/* TODO: implement arena_reset */
void arena_reset(Arena* arena)
{
    arena->offset = 0;
}

/* TODO: implement arena_free */
void arena_free(Arena* arena)
{
    free(arena->buffer);
    free(arena);
    arena = NULL;
}

/* TODO: implement run_alloc */
void run_alloc(Arena* arena)
{
    arena_alloc(arena, sizeof(int) * 10, 8);
    arena_alloc(arena, sizeof(double) * 5, 16);
    arena_alloc(arena, sizeof(char) * 100, 1);

    printf("Allocated %d bytes\n", arena->offset);
}

/* TODO: implement run_reset */
void run_reset(Arena* arena)
{
    run_alloc(arena);
    arena_reset(arena);
    printf("After reset: %d bytes\n", arena->offset);
}

/* TODO: implement run_align */
void run_align(Arena* arena)
{
    char* alloc1 = arena_alloc(arena, sizeof(int) * 10, 8);
    if (((uintptr_t)alloc1 - (uintptr_t)arena->buffer) % 8 == 0)
    {
        printf("8-byte aligned: YES\n");
    }

    char* alloc2 = arena_alloc(arena, sizeof(double) * 5, 16);
    if (((uintptr_t)alloc2 - (uintptr_t)arena->buffer) % 16 == 0)
    {
        printf("16-byte aligned: YES\n");
    }
}

/* TODO: implement run_oom */
void run_oom(Arena* arena)
{
    void* alloc = arena_alloc(arena, 1025, 8);
    if (alloc == NULL)
    {
        printf("Out of memory handled\n");
    }
}

int main(void) {
    Arena* arena;

    char mode[64] = "";
    if (scanf("%63s", mode) != 1) {
        mode[0] = '\0';
    }
    /* TODO: handle mode "alloc" (sample) and other modes */
    int is_all = strcmp(mode, "alloc") != 0 && strcmp(mode, "reset") != 0 && strcmp(mode, "align") != 0 && strcmp(mode, "oom") != 0;

    if (strcmp(mode, "alloc") == 0)
    {
        arena = arena_size(ARENA_SIZE);
        run_alloc(arena);
        arena_free(arena);
    }
    if (is_all || strcmp(mode, "reset") == 0)
    {
        arena = arena_size(ARENA_SIZE);
        run_reset(arena);
        arena_free(arena);
    }
    if (is_all || strcmp(mode, "align") == 0)
    {
        arena = arena_size(ARENA_SIZE);
        run_align(arena);
        arena_free(arena);
    }
    if (is_all || strcmp(mode, "oom") == 0)
    {
        arena = arena_size(ARENA_SIZE);
        run_oom(arena);
        arena_free(arena);
    }

    return 0;
}