#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <windows.h> // Thu vien bat buoc cho hieu ung Loading va doi mau chu

using namespace std;

#define MAX_V 100 

class GraphColoring {
private:
    int V, E;
    int adj[MAX_V][MAX_V]; 
    int colors[MAX_V];     
    int degree[MAX_V];     

    string colorNames[14] = {
        "Chua to", "Do", "Xanh la", "Xanh duong", "Vang", "Tim", 
        "Xanh lo", "Cam", "Hong", "Nau", "Xam", "Xanh ngoc", 
        "Tim than", "Vang chanh"
    };

public:
    // khoi tao do thi voi V dinh, mac dinh khong co canh va chua to mau
    GraphColoring(int v = 0) {
        this->V = v;
        for (int i = 0; i < MAX_V; i++) {
            colors[i] = -1;
            degree[i] = 0;
            for (int j = 0; j < MAX_V; j++) {
                adj[i][j] = 0;
            }
        }
    }

    void addEdge(int u, int v) {
        if (u >= 0 && u < V && v >= 0 && v < V) {
            if (adj[u][v] == 0) {
                adj[u][v] = adj[v][u] = 1;
                degree[u]++;
                degree[v]++;
            }
        }
    }
    
    // tinh do bao hoa cua dinh u
    int getSaturation(int u) {
        bool usedColors[MAX_V + 1];
        for (int i = 0; i <= MAX_V; i++) usedColors[i] = false;

        int count = 0;
        for (int v = 0; v < V; v++) {
            if (adj[u][v] == 1 && colors[v] != -1) {
                if (!usedColors[colors[v]]) {
                    usedColors[colors[v]] = true;
                    count++;
                }
            }
        }
        return count;
    }
    
    // cot loi chuong trinh DSATUR
    void runDSATUR() {
        int coloredCount = 0;
        while (coloredCount < V) {
            int bestV = -1;
            int maxSat = -1;
            int maxDeg = -1;
            
            // tim dinh co do bao hoa cao nhat, neu trung thi chon dinh co bac cao nhat
            for (int i = 0; i < V; i++) {
                if (colors[i] == -1) {
                    int curSat = getSaturation(i);
                    if (curSat > maxSat) {
                        maxSat = curSat;
                        maxDeg = degree[i];
                        bestV = i;
                    } else if (curSat == maxSat) {
                        if (degree[i] > maxDeg) {
                            maxDeg = degree[i];
                            bestV = i;
                        }
                    }
                }
            }
            
            // khoi tao mang danh dau mau da su dung cho cac dinh ke voi bestV
            bool available[MAX_V + 1];
            for (int i = 0; i <= MAX_V; i++) available[i] = true;
            
            // danh dau cac mau da su dung cho cac dinh ke voi bestV
            for (int v = 0; v < V; v++) {
                if (adj[bestV][v] == 1 && colors[v] != -1) {
                    available[colors[v]] = false;
                }
            }
            
            // tim mau nho nhat chua duoc su dung
            int clr;
            for (clr = 1; clr <= V; clr++) {
                if (available[clr]) break;
            }
            
            // to mau cho dinh bestV
            colors[bestV] = clr;
            coloredCount++;
        }
    }

    string getColorName(int idx) {
        if (idx > 0 && idx < 14) return colorNames[idx];
        return "Mau " + to_string(idx);
    }

    void printResult() {
        int maxClr = 0;
        cout << "\n========= KET QUA TO MAU =========" << endl;
        cout << left << setw(10) << "Dinh" << "Ten Mau" << endl;
        for (int i = 0; i < V; i++) {
            cout << left << setw(10) << i+1 << getColorName(colors[i]) << endl;
            if (colors[i] > maxClr) maxClr = colors[i];
        }
        cout << "------------------------------------------------" << endl;
        cout << "TONG SO MAU: " << maxClr << endl;
    }

    void saveToFile(string filename) {
        ofstream outFile(filename);
        if (!outFile) return;
        outFile << "KET QUA TO MAU\n";
        for (int i = 0; i < V; i++) {
            outFile << "Dinh " << i+1 << ": " << getColorName(colors[i]) << "\n";
        }
        outFile.close();
        cout << "-> Luu file " << filename << " thanh cong!" << endl;
        cout << "Nhan Enter de quay lai menu..."; cin.ignore(); cin.get();
    }
};

void subMenuProcess(GraphColoring &g) {
    int subChoice;
    while (true) {
        system("cls");
        system("color 0B"); // Tra man hinh ve mau xanh Cyan co ban
        
        cout << "==========================================================================" << endl;
        cout << "|                     MENU XU LY DO THI - DSATUR                         |" << endl;
        cout << "==========================================================================" << endl;
        
        //  Doi mau chu sang Vang (Mã mau 14) cho dong thong bao 
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, 14); 
        cout << "  (*) Thong bao: Da co du lieu do thi san sang de xu ly." << endl;
        SetConsoleTextAttribute(hConsole, 11); // Tra lai mau xanh Cyan (Mã 11)
        cout << "--------------------------------------------------------------------------" << endl;
        
        cout << "[1] Bat dau to mau do thi" << endl;
        cout << "[2] Quay lai phan nhap du lieu (Menu chinh)" << endl;
        cout << "[0] Thoat chuong trinh" << endl;
        cout << "Chon (1/2/0): "; cin >> subChoice;
        
        if (cin.fail()) {
            cin.clear(); cin.ignore(10000, '\n');
            continue;
        }
        
        if (subChoice == 1) {
            g.runDSATUR();
            g.printResult();
            g.saveToFile("output.txt"); 
        } 
        else if (subChoice == 2) {
            break; // Thoat khoi Menu phu, tro ve Menu chinh
        } 
        else if (subChoice == 0) {
            exit(0); // Thoat han chuong trinh
        }
    }
}

// =========================================================================
// HAM MAIN - CHUONG TRINH CHINH
// =========================================================================
int main() {
    int choice;
    while (true) {
        system("cls");
        system("color 0B");
        cout << "==========================================================================" << endl;
        cout << "|                                                                        |" << endl;
        cout << "|       CHUONG TRINH MO PHONG BAI TOAN TO MAU BAN DO BANG TO MAU         |" << endl;
        cout << "|                       CAC DINH CUA DO THI                              |" << endl;
        cout << "|                                                                        |" << endl;
        cout << "==========================================================================" << endl << endl;
        cout << "[1] Nhap tu file (input.txt)" << endl;
        cout << "[2] Nhap bang tay (Manual)" << endl;
        cout << "[0] Thoat chuong trinh" << endl;
        cout << "Chon (1/2/0): "; cin >> choice;

        int n, e;
        if (cin.fail()) {
            cin.clear(); cin.ignore(10000, '\n');
            cout << "(!) Loi: Vui long chi nhap con so!" << endl;
            cout << "Nhan Enter de chon lai..."; cin.get();
            continue;
        }
        
        switch (choice) {
        case 0:
            exit(0);
            break;
            
        case 1:
        {
            ifstream f("input.txt");
            if (!f) { 
                cout << "(!) Loi: Khong thay file!" << endl; 
                cout << "Nhan Enter de chon lai..."<< endl; 
                cin.ignore(); cin.get(); continue;
            }
            if (!(f >> n >> e)) {
                cout << "(!) Loi: Du lieu trong file khong hop le!" << endl; 
                cout << "Nhan Enter de chon lai..." << endl; 
                cin.ignore(); cin.get(); continue;
            }
            if (n <= 0 || n > MAX_V) {
                cout << "(!) Loi: So dinh phai tu 1 den " << MAX_V << "!" << endl; 
                cout << "Nhan Enter de chon lai..." << endl; 
                cin.ignore(); cin.get(); continue;
            }
            if (e < 0 || e > n * (n - 1) / 2) {
                cout << "(!) Loi: So canh khong hop le!" << endl; 
                cout << "Nhan Enter de chon lai..." << endl; 
                cin.ignore(); cin.get(); continue;
            }
            
            GraphColoring g(n);
            bool check = true;
            for (int i = 0; i < e; i++) {
                int u, v; 
                if (!(f >> u >> v)) {
                    check = false;
                    cout << "(!) Loi: Du lieu canh trong file khong hop le!" << endl; 
                    break;
                }
                if (u < 0 || u >= n || v < 0 || v >= n) {
                    check = false;
                    cout << "(!) Loi: Canh khong hop le!" << endl; 
                    break;
                }
                g.addEdge(u, v);
            }
            f.close();
            
            if (!check) {
                cout << "Nhan Enter de chon lai..." << endl; 
                cin.ignore(); cin.get();
                continue;
            }

            // --- HIEU UNG DANG TAI DU LIEU (LOADING ANIMATION) ---
            cout << "\nDang lay du lieu tu file input.txt";
            for(int i = 0; i < 6; i++) {
                cout << ".";
                Sleep(300); // Dung chuong trinh 300 milliseconds moi dau cham
            }
            
            cout << "\n\n=> THANH CONG: Da nap du lieu vao he thong!" << endl;
            cout << "Nhan Enter de tiep tuc..."; 
            cin.ignore(); cin.get();
            
            // goi menu phu
            subMenuProcess(g);
            break;
        }
        
        case 2:
        {
            cout << "Nhap so dinh: ";
            if (!(cin >> n)) {
                cin.clear(); cin.ignore(10000, '\n');
                cout << "(!) Loi: Vui long chi nhap con so!" << endl;
                cout << "Nhan Enter de chon lai..."; cin.get(); continue;
            } else if (n <= 0 || n > MAX_V) {
                cout << "(!) Loi: So dinh phai tu 1 den " << MAX_V << "!" << endl;
                cout << "Nhan Enter de chon lai..."; cin.ignore(); cin.get(); continue;
            }
            cout << "Nhap so canh: ";
            if (!(cin >> e)) { 
                cin.clear(); cin.ignore(10000, '\n');
                cout << "(!) Loi: Vui long chi nhap con so!" << endl;
                cout << "Nhan Enter de chon lai..."; cin.get(); continue;
            } else if (e < 0 || e > n * (n - 1) / 2) {
                cout << "(!) Loi: So canh khong hop le!" << endl;
                cout << "Nhan Enter de chon lai..."; cin.ignore(); cin.get(); continue;
            }
            
            GraphColoring g(n);
            for (int i = 0; i < e; i++) {
                int u, v; cin >> u >> v;
                g.addEdge(u, v);
            }
            
            // --- THONG BAO NHAP TAY THANH CONG ---
            cout << "\n=> THANH CONG: Da nap du lieu do thi vao he thong!" << endl;
            cout << "Nhan Enter de tiep tuc..."; 
            cin.ignore(); cin.get();
            
            // goi menu phu
            subMenuProcess(g);
            break;
        }
        default:
            cout << "(!) Loi: Lua chon khong hop le. Vui long chon 1 hoac 2." << endl;
            cout << "Nhan Enter de chon lai..."; cin.ignore(); cin.get();
            break;
        }
    }
    return 0;
}