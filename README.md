# University Library Management System in C++

A modular, CLI-based Library Management System implemented in C++, showcasing advanced data structures, algorithms, and file I/O management.
Designed for efficient book search, borrowing workflow, and recommendation systems using a real-world CSV dataset.

## Core Features

- Secure User Authentication

  - Student login via encrypted IDs
  
  - Fast lookup using binary search on sorted ID arrays
  
  - currentUser indexing allows direct mapping to user-specific data

- Book Data Management

  - Load and persist books in CSV format
  
  - Fields include ISBN, title, authors, categories, rating, pages, availability
  
  - Dynamic memory and struct-based modeling for scalable data representation

- Borrowing Workflow

  - Borrow/return operations with real-time availability tracking
  
  - Persistent borrowed-book tracking (borrowed.csv)
  
  - Efficient removal and update using linear search and file rewriting

- Advanced Search

  - Multi-criteria search: title, author, category, year, rating threshold
  
  - Supports partial matches, case-insensitive comparisons, and multi-author/category tokenization
  
  - Search results feed into borrow handling logic

- Recommendation Engine

  - Top-rated recommendations via bubble-sorted pointer arrays
  
  - Efficient traversal with Jump Search for scalable top-N book display
  
  - Optional circular linked list for continuous recommendation browsing

## Technical Highlights

- Algorithms

  - Linear search for flexible string queries
  
  - Binary search for high-performance authentication
  
  - Jump search for optimized recommendation traversal
  
  - Bubble sort on pointer arrays for minimal data copying

- Data Structures

  - Struct-based book model
  
  - 2D dynamic arrays for user-borrowed books
  
  - Circular linked list for recommendation traversal

- File Handling

  - Robust CSV parsing (parseField) with support for quoted/unquoted fields
  
  - Persistent storage of borrowed books and book metadata
  
  - Safe memory handling and input normalization (cleanString, toLowerStr)

- Modularity & Scalability

  - Separation of concerns: books.cpp, recommendations.cpp, and main menu logic
  
  - Pointer-based manipulation minimizes memory footprint
  
  - Scales efficiently to hundreds of books and multiple users

## Architecture Overview
- main.cpp              → CLI and menu handling  
- books.cpp / books.h   → Book CRUD, parsing, search, borrow/return  
- recommendations.cpp   → Top-rated books, Jump Search, circular linked list  
- borrowed.csv          → Persistent borrowing records  
- books.csv             → Book dataset  


## Developer Insights

- Designed for efficiency: sorting pointers instead of objects, optimized search algorithms

- Handles edge cases gracefully: unavailable books, invalid input, and empty datasets

- Demonstrates full-stack systems thinking: data persistence, memory management, algorithms, and UX in CLI
