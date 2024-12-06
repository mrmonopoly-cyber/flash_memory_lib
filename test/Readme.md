# Flash Memory Lib Tests

## Requirements

- [Make](https://www.gnu.org/software/make/)
- [GCC](https://gcc.gnu.org/)

## Run
 
 You can choose to run all the tests at once or run specific tests 
 individually

### All test
To run all tests at in once run the following command
```bash
    ./run_tests.sh
```

### One test
Enter the directory of the tests and choose the test to execute, example:

```sh
    cd ./tests
    cd ./init
```

Run the test:
```sh
    ./run_test.sh
```

### Contribution
If you want to add a new test run the following command:
```sh
    ./add_test.sh [name]
```
If you want to remove a test run the following command:
```sh
    ./rm_test.sh [name]
```
If you want to list the available tests run the following command 
```sh
    ./list_tests.sh
```
