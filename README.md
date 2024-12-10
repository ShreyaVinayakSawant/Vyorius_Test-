# Vyorius_Test-
# Movie Recommendation System

This project is a simple movie recommendation system that predicts the top-rated movies for a specific user based on their and other users' ratings using collaborative filtering techniques. It includes an implementation in C++ and outputs recommendations in an HTML format.

## Features
- **Collaborative Filtering**: Calculates cosine similarity between users to predict ratings for movies.
- **CSV File Parsing**: Reads user ratings from a CSV file containing movie ratings.
- **HTML Output**: Generates a user-friendly HTML file displaying the top recommended movies for a target user.

## Files in the Repository
1. `movie_recommendation.cpp`: The main program that implements the recommendation system logic.
2. `very_large_ratings_dataset.csv`: A CSV file containing the movie ratings data. Each row represents a user, and each column represents a movie.
3. `recommendations.html`: A sample HTML output showing the top 5 movie recommendations for a specific user.

## Getting Started

### Prerequisites
- A C++ compiler (e.g., GCC)
- Basic understanding of C++ programming
- A text editor or IDE (e.g., Visual Studio Code)

### Running the Project
1. **Compile the C++ File**:
   ```bash
   g++ movie_recommendation.cpp -o movie_recommendation
