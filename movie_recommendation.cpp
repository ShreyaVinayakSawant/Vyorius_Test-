#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <stdexcept>

// Function to load the ratings matrix from a CSV file
std::vector<std::vector<int>> loadRatingsMatrix(const std::string& filename) {
    std::vector<std::vector<int>> ratings;
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open()) {
        throw std::runtime_error("Error opening file: " + filename);
    }

    int lineNumber = 0;
    while (std::getline(file, line)) {
        lineNumber++;
        // Skip the first row containing movie names
        if (lineNumber == 1) continue;

        std::stringstream ss(line);
        std::string value;
        std::vector<int> row;

        while (std::getline(ss, value, ',')) {
            try {
                row.push_back(std::stoi(value)); // Convert string to integer
            } catch (const std::exception&) {
                row.push_back(0); // Handle invalid values by setting to 0
            }
        }

        if (!row.empty()) {
            ratings.push_back(row);
        } else {
            std::cerr << "Warning: Empty row at line " << lineNumber << "\n";
        }
    }

    if (ratings.empty()) {
        throw std::runtime_error("Error: No valid data found in file.");
    }
    return ratings;
}

// Calculate cosine similarity between two users
double calculateSimilarity(const std::vector<int>& user1, const std::vector<int>& user2) {
    double dotProduct = 0.0, normA = 0.0, normB = 0.0;
    for (size_t i = 0; i < user1.size(); i++) {
        if (user1[i] != 0 && user2[i] != 0) {
            dotProduct += user1[i] * user2[i];
            normA += user1[i] * user1[i];
            normB += user2[i] * user2[i];
        }
    }
    return (normA && normB) ? dotProduct / (std::sqrt(normA) * std::sqrt(normB)) : 0.0;
}

// Predict ratings for a target user
std::vector<std::pair<int, double>> predictRatings(
    const std::vector<std::vector<int>>& ratings, int targetUser, int topN) {
    std::vector<double> predictedRatings(ratings[0].size(), 0.0);
    std::vector<double> similaritySums(ratings[0].size(), 0.0);

    for (size_t otherUser = 0; otherUser < ratings.size(); otherUser++) {
        if (otherUser == targetUser) continue;

        double similarity = calculateSimilarity(ratings[targetUser], ratings[otherUser]);
        for (size_t movie = 0; movie < ratings[0].size(); movie++) {
            if (ratings[targetUser][movie] == 0 && ratings[otherUser][movie] != 0) {
                predictedRatings[movie] += similarity * ratings[otherUser][movie];
                similaritySums[movie] += std::abs(similarity);
            }
        }
    }

    std::vector<std::pair<int, double>> recommendations;
    for (size_t i = 0; i < predictedRatings.size(); i++) {
        if (ratings[targetUser][i] == 0 && similaritySums[i] != 0) {
            recommendations.emplace_back(i, predictedRatings[i] / similaritySums[i]);
        }
    }

    std::sort(recommendations.begin(), recommendations.end(),
              [](const auto& a, const auto& b) { return b.second > a.second; });

    if (recommendations.size() > topN) {
        recommendations.resize(topN);
    }
    return recommendations;
}

// Generate an HTML file with the recommendations
void generateHTML(const std::vector<std::pair<int, double>>& recommendations, int targetUser, int topN) {
    std::ofstream htmlFile("recommendations.html");

    if (!htmlFile.is_open()) {
        std::cerr << "Error: Unable to create HTML file.\n";
        return;
    }

    htmlFile << "<!DOCTYPE html>\n<html>\n<head>\n<title>Movie Recommendations</title>\n";
    htmlFile << "<style>body { font-family: Arial, sans-serif; } table { width: 50%; margin: auto; border-collapse: collapse; } ";
    htmlFile << "th, td { border: 1px solid #ddd; padding: 8px; text-align: center; } th { background-color: #f4f4f4; }</style>\n";
    htmlFile << "</head>\n<body>\n";
    htmlFile << "<h2 style='text-align: center;'>Top " << topN << " Recommended Movies for User " << targetUser + 1 << "</h2>\n";
    htmlFile << "<table>\n<tr><th>Rank</th><th>Movie ID</th><th>Predicted Rating</th></tr>\n";

    int rank = 1;
    for (const auto& rec : recommendations) {
        htmlFile << "<tr><td>" << rank++ << "</td><td>Movie " << rec.first + 1
                 << "</td><td>" << rec.second << "</td></tr>\n";
    }

    htmlFile << "</table>\n</body>\n</html>";
    htmlFile.close();

    std::cout << "HTML file 'recommendations.html' generated successfully. Open it in a browser to view.\n";
}

// Main function
int main() {
    const std::string filename = "very_large_ratings_dataset.csv";
    const int targetUser =2; // Target user index (0-based)
    const int topN = 5;       // Number of recommendations

    try {
        std::vector<std::vector<int>> ratings = loadRatingsMatrix(filename);

        if (targetUser >= ratings.size()) {
            throw std::out_of_range("Target user index is out of range.");
        }

        std::vector<std::pair<int, double>> recommendations = predictRatings(ratings, targetUser, topN);

        generateHTML(recommendations, targetUser, topN);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}
