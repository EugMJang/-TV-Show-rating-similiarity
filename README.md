# "TV show" rating similarity

This project analyzes the similarity of "tv shows" taken directly from MyAnimeList by reading the rating of 1001 anime spanning across every user and calculating the average difference in rating between the two shows.

## Installation
Download these files as a zip file and unzip them. Make sure to have clang++ and make installed or else this project will not work.

## Usage
Assuming everything is installed correctly, run the following command:
```
make main
./main
```
The program will proceed to run the reading of the CSV files (type Y when prompted to run the project with pre-read CSVs as reading the csvs themselves takes a while) creating the Force-Directed Graph (type the number of iterations to run Force-Directed Graph when prompted), BFS (type the id of the anime you wish to run BFS on, the ids and animes are in the file Data/id_map.csv), and Dijkstra's (type the start and end ids when prompted).

Final graph visualization:
![image](https://user-images.githubusercontent.com/18586404/148635973-6847c892-1b03-4e4e-a935-4945820147b3.png)
