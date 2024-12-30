# File Search Server 

This project is a multithreaded web server designed to manage an inverted index for text file search and retrieval. It supports efficient file indexing, concurrent client handling, and seamless file management.

---

## Features

- **Search API**: Quickly search for documents containing specific keywords.
- **Download API**: Retrieve the content of indexed documents by file ID.
- **Multithreading**: Handles multiple client requests concurrently.
- **Dynamic Index Updates**: Automatically updates the inverted index when files are added, modified, or deleted.
- **Thread Pool**: Optimizes resource usage for high-performance multithreading.

---

## API Protocol

### 1. Search Endpoint
- **Path**: `/search`
- **Method**: `GET`
- **Query Parameters**:
    - `q` (required): Keywords to search for. If there are more than one values, combine them with "+".
- **Response**:
    - **200 OK**:
      ```json
      {
        "status": "success",
        "fileIDs": [1, 2, 5]
      }
      ```
    - **400 Bad Request** (if `q` is missing):
      ```json
      {
        "status": "error",
        "message": "Parameter (q) must be provided for search."
      }
      ```

### 2. Download Endpoint
- **Path**: `/download`
- **Method**: `GET`
- **Query Parameters**:
    - `id` (required): ID of the file to download.
- **Response**:
    - **200 OK**:
      ```json
      {
        "status": "success",
        "fileName": "example.txt",
        "content": "File content here..."
      }
      ```
    - **400 Bad Request** (if `id` is missing or invalid):
      ```json
      {
        "status": "error",
        "message": "Parameter (id) must be provided for download."
      }
      ```
    - **400 Bad Request** (if no file exists for the given ID):
      ```json
      {
        "status": "error",
        "message": "No file with such ID exists."
      }
      ```

### 3. 404 Error
- Returned for all unrecognized paths.
- **Response**:
  ```json
  {
    "status": "error",
    "message": "No such uri path exists."
  }

## Building the Project

### Requirements
- A C++ compiler that supports C++17 (e.g., GCC, Clang, or MSVC)
- CMake version 3.27 or higher
- Make (for Linux/macOS) or an equivalent build system
- Git to clone the repository
- A terminal or command prompt for running commands

### Steps to Build
1. Clone the repository to your local machine:
   ```bash
   git clone https://https://github.com/alinapavlyuk/course-work-parallel-computing
   cd course-work-parallel-computing
2. Create a build directory and navigate into it:
   ```bash
   mkdir build
   cd build
3. Run CMake to configure the build system:
   ```bash
   cmake ..
   ```
   This will generate the necessary build files based on the CMakeLists.txt configuration.

4. Build the project using make (or an alternative build system, depending on your setup):
   ```bash
   make
   ```
   This will compile the project and generate the executable.

### Running the Project
Once the build is complete, you can run the project using the generated executable.
   ```bash
   ./course_work
   ```


