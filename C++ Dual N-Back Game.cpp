#include <iostream>
#include <windows.h>
#include <MMsystem.h>
#include <string>

// ZAHIRUL ARIEF IRFAN 1201103516
// MOUSA SUFIAN 1201102569

using namespace std;

class Display {
public:
    Display() {}

    string board[9] = { " ", " ", " ", " ", " ", " ", " ", " " };

    void draw() {
        cout << "     |     |      \n";
        cout << "  " << board[0] << "  |  " << board[1] << "  |  " << board[2] << "\n";
        cout << "_____|_____|_____ \n";
        cout << "     |     |      \n";
        cout << "  " << board[3] << "  |  " << board[4] << "  |  " << board[5] << "\n";
        cout << "_____|_____|_____ \n";
        cout << "     |     |      \n";
        cout << "  " << board[6] << "  |  " << board[7] << "  |  " << board[8] << "\n";
        cout << "     |     |      \n";
        cout << "\n";
    }

    void clearBoard() {
        for (int i = 0; i < 9; i++) {
            board[i] = " ";
        }
    }

    void clearScreen() {
        cout << "\033[2J\033[1;1H"; // acts like a clear screen command
    }
};

class User {
private:
    char Player;
    char AudioPlayer;
    int Score;
    int ScoreLimit;
    int N;

public:
    User(int S = 0, int SL = 2, int n = 1) {
        Score = S;
        ScoreLimit = SL;
        N = n;
    }

    void scoreThresh() {
        if (Score == ScoreLimit) {
            ScoreLimit += 2;
            N++;
        }
    }

    void playerInput() {
        cin >> Player;
    }

    char getPlayer() {
        return Player;
    }

    char getAudioPlayer() {
        return AudioPlayer;
    }

    int getScore() {
        return Score;
    }

    int getN() {
        return N;
    }

    void changeScore(int set) {
        if (set == 0)
            Score++;
        else if (set == 1 && Score > 0)
            Score--;  // only decrement if score is greater than 0 to prevent negative scores
    }
};

class Game {
private:
    int Looping;
    int* StoreNum;
    int* StoreAudio;
    Display Display1;
    User User1;

public:
    Game(int Loop = 1000) {
        Looping = Loop;
        StoreNum = new int[Looping]();
        StoreAudio = new int[Looping]();
    }

    ~Game() {
        delete[] StoreNum;
        delete[] StoreAudio;
    }

    void playSound(int soundIndex) {
        // Assuming you have sound files named sound0.wav, sound1.wav, ..., sound7.wav (SoundIndex acts like our 'i' in the for loop, so sounds0-7)
        string soundFile = "sound" + to_string(soundIndex) + ".wav";
        // Convert to a wide string (this is necessary because the playsound function only works with wide strings)
        wstring wideSoundFile(soundFile.begin(), soundFile.end());
        PlaySoundW(wideSoundFile.c_str(), NULL, SND_ASYNC);
    }

    void runGame() {
        for (int Round = 0; Round <= Looping; Round++) {
            cout << "Score: " << User1.getScore() << endl;

            StoreNum[Round] = rand() % 9;
            StoreAudio[Round] = rand() % 9;

            Display1.board[StoreNum[Round]] = "X";

            playSound(StoreAudio[Round]);

            Display1.draw();

            cout << "Does the Position/audio match " << User1.getN() << "-step back? (choose 1-4):"<<endl;
            cout << "\n1. yes/no\t2. yes/yes\n3. no/no\t4. no/yes\n\nYour input: ";
            int input;
            cin >> input;

            bool positionMatch = (StoreNum[Round] == StoreNum[Round - User1.getN()]);
            bool audioMatch = (StoreAudio[Round] == StoreAudio[Round - User1.getN()]);

            switch (input) {
            case 1: // yes/no
                if (positionMatch && !audioMatch) {
                    User1.changeScore(0); // Increment score
                }
                else {
                    User1.changeScore(1); // Decrement score
                }
                break;
            case 2: // yes/yes
                if (positionMatch && audioMatch) {
                    User1.changeScore(0); // Increment score
                }
                else {
                    User1.changeScore(1); // Decrement score
                }
                break;
            case 3: // no/no
                if (!positionMatch && !audioMatch) {
                    // Correctly identifying no match should not increment or decrement the score.
                }
                else {
                    User1.changeScore(1); // Decrement score
                }
                break;
            case 4: // no/yes
                if (!positionMatch && audioMatch) {
                    User1.changeScore(0); // Increment score
                }
                else {
                    User1.changeScore(1); // Decrement score
                }
                break;
            default:
                cout << "Invalid input" << endl;
                break;
            }

            Display1.clearBoard();
            User1.scoreThresh();
            Display1.clearScreen();
        }
    }
};

int main() {
    srand(time(0));
    Game Game1;

    system("color 17");

    cout << "===Welcome to our Dual N-Back Game!===" << endl;
    Game1.runGame();

    return 0;
}