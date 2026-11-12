# Interactive People Dictionary

A command-line application written in C for managing a small collection of people. Each record stores a person's name, CPF, and age.

## Features

- Insert a new person.
- Remove a person using their CPF.
- Search for a person by CPF.
- List the registered records.
- Keep records ordered by CPF.
- Use binary search to find records efficiently.
- Check CPF input formatting and the expected 11-digit numeric length.

The application stores up to 100 records in a vector. CPF validation checks the input format and length; it does not perform the official CPF check-digit verification. Data is kept in memory while the program is running and is not saved to a file.

## Build and run

Requirements: GCC or another C compiler compatible with C11.

```bash
gcc -std=c11 -Wall -Wextra pessoas.c -o dicionario
./dicionario
```

## Menu

The interactive menu provides the following operations:

| Option | Operation |
| --- | --- |
| `i` | Insert a person |
| `r` | Remove a person |
| `b` | Search by CPF |
| `p` | Print/list records |
| `s` | Exit |

## Technologies and concepts

- C (C11)
- Structs and vectors
- Ordered data
- Binary search
- Input validation

---

Academic project focused on data structures and record management.
