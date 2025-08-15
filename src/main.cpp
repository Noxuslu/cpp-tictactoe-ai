#include <bits/stdc++.h>
using namespace std;

// -------- Tic-Tac-Toe with unbeatable AI (Minimax + Alpha-Beta) --------
// Human: 'X'  |  AI: 'O'

struct Move { int idx; int score; };

static const int LINES[8][3] = {
    {0,1,2},{3,4,5},{6,7,8}, // rows
    {0,3,6},{1,4,7},{2,5,8}, // cols
    {0,4,8},{2,4,6}          // diags
};

char winner(const array<char,9>& b) {
    for (auto &L : LINES) {
        if (b[L[0]]!=' ' && b[L[0]]==b[L[1]] && b[L[1]]==b[L[2]])
            return b[L[0]];
    }
    bool full = true;
    for (char c : b) if (c==' ') { full=false; break; }
    if (full) return 'D'; // draw
    return ' '; // ongoing
}

void printBoard(const array<char,9>& b) {
    auto cell = [&](int i){
        return b[i]==' ' ? char('1'+i) : b[i];
    };
    cout << "\n";
    cout << " " << cell(0) << " | " << cell(1) << " | " << cell(2) << "\n";
    cout << "---+---+---\n";
    cout << " " << cell(3) << " | " << cell(4) << " | " << cell(5) << "\n";
    cout << "---+---+---\n";
    cout << " " << cell(6) << " | " << cell(7) << " | " << cell(8) << "\n\n";
}

vector<int> emptyCells(const array<char,9>& b) {
    vector<int> v;
    for (int i=0;i<9;i++) if (b[i]==' ') v.push_back(i);
    return v;
}

// Minimax with alpha-beta pruning
int minimax(array<char,9>& b, bool aiTurn, int depth, int alpha, int beta) {
    char w = winner(b);
    if (w=='O') return 10 - depth;   // AI wins (prefer faster win)
    if (w=='X') return depth - 10;   // Human wins (prefer slower loss)
    if (w=='D') return 0;

    auto moves = emptyCells(b);
    if (aiTurn) {
        int best = INT_MIN;
        for (int idx : moves) {
            b[idx] = 'O';
            best = max(best, minimax(b, false, depth+1, alpha, beta));
            b[idx] = ' ';
            alpha = max(alpha, best);
            if (beta <= alpha) break;
        }
        return best;
    } else {
        int best = INT_MAX;
        for (int idx : moves) {
            b[idx] = 'X';
            best = min(best, minimax(b, true, depth+1, alpha, beta));
            b[idx] = ' ';
            beta = min(beta, best);
            if (beta <= alpha) break;
        }
        return best;
    }
}

int bestMove(array<char,9>& b) {
    int bestScore = INT_MIN;
    int moveIdx = -1;
    for (int idx : emptyCells(b)) {
        b[idx] = 'O';
        int score = minimax(b, false, 0, INT_MIN/2, INT_MAX/2);
        b[idx] = ' ';
        if (score > bestScore) {
            bestScore = score;
            moveIdx = idx;
        }
    }
    return moveIdx;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cout << "=== Tic-Tac-Toe (X-O) | C++ Minimax AI ===\n";
    cout << "Sen: X  |  Yapay Zekâ: O\n";
    cout << "Kare seçmek için 1-9 rakamlarını kullan.\n";

    while (true) {
        array<char,9> board{}; board.fill(' ');
        bool humanTurn = true; // İnsan başlar
        char w = ' ';

        while ((w = winner(board))==' ') {
            printBoard(board);
            if (humanTurn) {
                cout << "Hamlen (1-9): ";
                string s; 
                if (!getline(cin, s)) return 0;
                if (s.size()==0) continue;
                if (!isdigit(s[0])) { cout << "Lütfen 1-9 arası bir sayı.\n"; continue; }
                int pos = s[0]-'1';
                if (pos<0 || pos>8) { cout << "Geçersiz kare.\n"; continue; }
                if (board[pos] != ' ') { cout << "Dolu! Başka kare seç.\n"; continue; }
                board[pos] = 'X';
            } else {
                int mv = bestMove(board);
                if (mv==-1) break;
                board[mv] = 'O';
                cout << "AI hamlesi: " << (mv+1) << "\n";
            }
            humanTurn = !humanTurn;
        }

        printBoard(board);
        if (w=='X') cout << "Tebrikler, kazandın! 🎉\n";
        else if (w=='O') cout << "AI kazandı. Yenilmez! 🤖\n";
        else cout << "Berabere! 🤝\n";

        cout << "Tekrar oyna? (y/n): ";
        string ans; 
        if (!getline(cin, ans)) break;
        if (ans.size()==0 || (ans[0]!='y' && ans[0]!='Y')) break;
    }

    cout << "Oyun bitti. Görüşürüz!\n";
    return 0;
}
