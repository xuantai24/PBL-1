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

    void exportDOT(string filename){
        ofstream f(filename);
        if (!f) {
            cout << "Khong the tao file DOT!\n";
            return;
        }

        f << "graph G {\n";

        // In đỉnh + màu
        for (int i = 1; i <= V; i++) {
            f << i << " [style=filled, fillcolor=";

            switch (colors[i-1]) {
                case 1: f << "red"; break;
                case 2: f << "green"; break;
                case 3: f << "blue"; break;
                case 4: f << "yellow"; break;
                case 5: f << "orange"; break;
                case 6: f << "pink"; break;
                default: f << "gray";
            }

            f << "];\n";
        }

        for (int i = 1; i <= V; i++) {
            for (int j = i + 1; j <= V; j++) {
                if (adj[i-1][j-1]) {
                    f << i << " -- " << j << ";\n";
                }
            }
        }

        f << "}\n";
        f.close();
    }

    void drawGraph() {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14); // Mau vang cho thong bao
            cout << "\nBan co muon xem do thi duoc to mau khong? (Y/N): ";
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10); // Tra lai mau xanh
            char ans; cin >> ans;
            if (ans != 'Y' && ans != 'y') return;

            exportDOT("graph.dot");

            // tạo ảnh
            system("dot -Tpng graph.dot -o graph.png");

            // mở ảnh (Windows)
            system("start graph.png");
            cout << "Nhan Enter de quay lai menu..."; cin.ignore(); cin.get();

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
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14); // Mau vang cho thong bao
        cout << "TONG SO MAU: " << maxClr << endl;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10); // Tra lai mau xanh 
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
    }
};

void subMenuProcess(GraphColoring &g) {
    int subChoice;
    while (true) {
        system("cls");
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, 11);
        cout << "==========================================================================" << endl;
        cout << "|                     MENU XU LY DO THI - DSATUR                         |" << endl;
        cout << "==========================================================================" << endl;
        
        //  Doi mau chu sang Vang (Mã mau 14) cho dong thong bao 
        SetConsoleTextAttribute(hConsole, 14); 
        cout << "  (*) Thong bao: Da co du lieu do thi. San sang de xu ly." << endl;
        SetConsoleTextAttribute(hConsole, 10);
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
            g.drawGraph();
        } 
        else if (subChoice == 2) {
            break; // Thoat khoi Menu phu, tro ve Menu chinh
        } 
        else if (subChoice == 0) {
            exit(0); // Thoat han chuong trinh
        }
    }
}

//------------------------------ CHUONG TRINH CHINH -----------------------------//--
int main() {
    int choice;
    while (true) {
        system("cls");
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, 11); // Mau xanh Cyan cho menu chinh
        cout << "==========================================================================" << endl;
        cout << "|                                                                        |" << endl;
        cout << "|       CHUONG TRINH MO PHONG BAI TOAN TO MAU BAN DO BANG TO MAU         |" << endl;
        cout << "|                       CAC DINH CUA DO THI                              |" << endl;
        cout << "|                                                                        |" << endl;
        cout << "==========================================================================" << endl << endl;
        SetConsoleTextAttribute(hConsole, 10); // Mau xanh la cho cac lua chon
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
                cout << "Nhan Enter de quay lai menu..."<< endl; 
                cin.ignore(); cin.get(); continue;
            }
            if (!(f >> n >> e)) {
                cout << "(!) Loi: Du lieu trong file khong hop le!" << endl; 
                cout << "Nhan Enter de quay lai menu..." << endl; 
                cin.ignore(); cin.get(); continue;
            }
            if (n <= 0 || n > MAX_V) {
                cout << "(!) Loi: So dinh phai tu 1 den " << MAX_V << "!" << endl; 
                cout << "Nhan Enter de quay lai menu..." << endl; 
                cin.ignore(); cin.get(); continue;
            }
            if (e < 0 || e > n * (n - 1) / 2) {
                cout << "(!) Loi: So canh khong hop le!" << endl; 
                cout << "Nhan Enter de quay lai menu..." << endl; 
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
                cout << "Nhan Enter de quay lai menu..." << endl; 
                cin.ignore(); cin.get();
                continue;
            }

            // HIEU UNG DANG TAI DU LIEU (LOADING ANIMATION)
            cout << "\nDang lay du lieu tu file input.txt";
            for(int i = 0; i < 6; i++) {
                cout << ".";
                Sleep(300); // Dung chuong trinh 300 milliseconds moi dau cham
            }
            
            SetConsoleTextAttribute(hConsole, 14); // Mau vang cho thong bao
            cout << "\n\n=> THANH CONG: Da nap du lieu vao he thong!" << endl;
            SetConsoleTextAttribute(hConsole, 10); // Tra lai mau xanh la
            cout << "Nhan Enter de tiep tuc..."; 
            cin.ignore(); cin.get();
            
            // goi menu phu
            subMenuProcess(g);
            break;
        }
        
        case 2:
        {
            system("cls");
            SetConsoleTextAttribute(hConsole, 14);
            cout << "==========================================================================" << endl;
            cout << "|                                                                        |" << endl;
            cout << "|                      NHAP DU LIEU DO THI BANG TAY                      |" << endl;
            cout << "|                 (Go phim [q] de huy va quay lai menu...)               |" << endl;
            cout << "==========================================================================" << endl << endl;
            SetConsoleTextAttribute(hConsole,10);

            string inputStr;

            cout << "Nhap so dinh (Nhap 'q' de huy va quay lai Menu chinh): ";
            cin >> inputStr;
            
            // Kiem tra thoat
            if (inputStr == "q" || inputStr == "Q") {
                cout << "\n-> Da huy lenh. Dang quay lai Menu chinh..." << endl;
                Sleep(800);
                break; 
            }
            
            // Co gang chuyen chuoi thanh so
            try {
                n = stoi(inputStr);
                if (n <= 0 || n > MAX_V) {
                    cout << "(!) Loi: So dinh phai tu 1 den " << MAX_V << "!" << endl;
                    cout << "Nhan Enter de chon lai..."; cin.ignore(10000, '\n'); cin.get(); continue;
                }
            } catch (...) {
                // Bat loi neu nguoi dung nhap chu ma khong phai 'q' (VD: 'abc')
                cout << "(!) Loi: Vui long chi nhap con so hoac 'q'!" << endl;
                cout << "Nhan Enter de chon lai..."; cin.ignore(10000, '\n'); cin.get(); continue;
            }

            int maxEdges = n * (n - 1) / 2;
            cout << "Nhap so canh (Toi da " << maxEdges << ", hoac 'q' de huy va quay lai Menu chinh): ";
            cin >> inputStr;
            
            if (inputStr == "q" || inputStr == "Q") {
                cout << "\n-> Da huy lenh. Dang quay lai Menu chinh..." << endl;
                Sleep(800);
                break;
            }
            
            try {
                e = stoi(inputStr);
                if (e < 0 || e > maxEdges) {
                    cout << "(!) Loi: So canh khong hop le!" << endl;
                    cout << "Nhan Enter de chon lai..."; cin.ignore(10000, '\n'); cin.get(); continue;
                }
            } catch (...) {
                cout << "(!) Loi: Vui long chi nhap con so hoac 'q'!" << endl;
                cout << "Nhan Enter de chon lai..."; cin.ignore(10000, '\n'); cin.get(); continue;
            }
            
            GraphColoring g(n);
            bool isCancelled = false; // danh dau thoat giua chung

            SetConsoleTextAttribute(hConsole, 14);
            if (e > 0) {
                cout << "\n==================================================" << endl;
                cout << "             HUONG DAN NHAP CANH DO THI           " << endl;
                cout << "==================================================" << endl;
                cout << " - Do thi hien tai co " << n << " dinh (Tu 1 den " << n << ")." << endl;
                cout << " - Nhap 2 so (u v) de tao canh (Vi du: 1 2)." << endl;
                cout << " - (!) Go phim [q] vao bat cu luc nao de HUY va thoat." << endl;
                cout << "--------------------------------------------------" << endl;
            SetConsoleTextAttribute(hConsole, 10);

                for (int i = 0; i < e; i++) {
                    string strU, strV; 
                    while (true) {
                        cout << " -> Nhap canh thu " << i + 1 << "/" << e << " (u v hoac q): ";
                        cin >> strU;
                        
                        if (strU == "q" || strU == "Q") {
                            isCancelled = true; 
                            break; 
                        }
                        
                        cin >> strV;
                        
                        if (strV == "q" || strV == "Q") {
                            isCancelled = true;
                            break;
                        }

                        try {
                            int u = stoi(strU);
                            int v = stoi(strV);

                            if (u < 1 || u > n || v < 1 || v > n) {
                                cout << "    (!) Loi: Dinh phai tu 1 den " << n << ". Nhap lai!" << endl;
                                continue;
                            }
                            if (u == v) {
                                cout << "    (!) Loi: Khong duoc noi dinh voi chinh no. Nhap lai!" << endl;
                                continue;
                            }
                            
                            g.addEdge(u - 1, v - 1);
                            break; 
                            
                        } catch (...) {
                            cout << "    (!) Sai dinh dang! Vui long nhap so hoac 'q'." << endl;
                        }
                    }
                    
                    if (isCancelled) break;
                }
            }
            
            if (isCancelled) {
                cout << "\n-> Da huy nhap canh. Dang quay lai Menu chinh..." << endl;
                Sleep(800);
                break;
            }
            
            // --- THONG BAO NHAP TAY THANH CONG ---
            SetConsoleTextAttribute(hConsole, 14); // Mau vang cho thong bao
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