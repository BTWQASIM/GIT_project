# GitHub Simulation

## Overview

This C++ program implements a user management system and a repository system allowing users to create, delete, and search repositories.

## Features

- **User Registration:** Users can register with a unique username and password.
- **User Authentication:** Registered users can login using their username and password.
- **User Profile Management:** Users can view and edit their profiles, including their followers and following.
- **Repository Management:** Users can create repositories with commit data and file information, delete repositories they own, and search for existing repositories.
- **Data Persistence:** User registration credentials and repository data are stored in CSV files for persistence between program executions.

## Dependencies

- This program uses C++ standard libraries and does not require any external dependencies.
- The program is designed to run on a Unix-like environment due to the use of system-specific commands like `clear`.

## Usage

1. Compile the program using a C++ compiler.
2. Execute the compiled binary.
3. Follow the on-screen instructions to register or login as a user, manage user profiles, and manage repositories.

## File Structure

- `main.cpp`: Contains the main program logic, including user interface and interaction.
- `HashTable.cpp`: Implements a hash table for user management.
- `SocialGraph.cpp`: Implements a social graph for managing user relationships (followers/following).
- `RepositoryTree.cpp`: Implements a binary search tree for managing repositories.
- `loginCredential.csv`: Stores registered user credentials (username, password).
- `repositories.csv`: Stores a list of repository filenames.
- `*.csv`: Each repository has its own CSV file storing repository details (username, repository name, commit data, file).

## Contributors

- Qasim Zubair

## License

This project is licensed under the [MIT License](LICENSE).
