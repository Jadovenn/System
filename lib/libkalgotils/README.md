
# libkalgotils

libkalgotils stand for "The kernel's utils and algorithms library".

## List

Create your structure with the LIST_HEAD member to use the list's functions.

```C
typedef struct Number_s {
    LIST_BLOCK_HEAD;
    int        integer;
}              Number_t;
```

 | function | complexity |
 | -----------|----------|
 | push_front |  <span class="text-success"> **O(1)** </span> |
 | pop_front  |  <span class="text-success"> **O(1)** </span> |
 | push_back  |  <span class="text-success"> **O(1)** </span> |
 | pop_back   |  <span class="text-success"> **O(1)** </span> |
 | at         |  <span class="text-warning"> **O(N)** </span> |
 | insert     |  <span class="text-warning"> **O(N)** </span> |
 | remove_at  |  <span class="text-warning"> **O(N)** </span> |
 | find_if    |  <span class="text-warning"> **O(N)** </span> |
 