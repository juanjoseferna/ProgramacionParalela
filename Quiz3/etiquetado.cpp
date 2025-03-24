#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

const int SIZE = 10;

class UnionFind {
public:
    vector<int> parent, rank;

    UnionFind(int n) : parent(n), rank(n, 0) {
        for (int i = 0; i < n; i++) parent[i] = i;
    }

    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }

    void unite(int x, int y) {
        int rootX = find(x), rootY = find(y);
        if (rootX != rootY) {
            if (rank[rootX] > rank[rootY]) parent[rootY] = rootX;
            else {
                parent[rootX] = rootY;
                if (rank[rootX] == rank[rootY]) rank[rootY]++;
            }
        }
    }
};

vector<vector<int>> generarImagen(int size) {
    vector<vector<int>> img(size, vector<int>(size));
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            img[i][j] = rand() % 2;  // 0 o 1
    return img;
}

void etiquetadoSerial(vector<vector<int>> &img, vector<vector<int>> &labels) {
    int size = img.size();
    int label = 1;

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (img[i][j] == 1 && labels[i][j] == 0) {
                labels[i][j] = label;
                label++;
            }
            if (i > 0 && labels[i - 1][j] > 0) labels[i][j] = labels[i - 1][j];
            if (j > 0 && labels[i][j - 1] > 0) labels[i][j] = labels[i][j - 1];
        }
    }
}

void etiquetadoParalelo(vector<vector<int>> &img, vector<vector<int>> &labels) {
    int size = img.size();
    UnionFind uf(size * size);
    
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (img[i][j] == 1) {
                int id = i * size + j;
                if (i > 0 && img[i - 1][j] == 1) uf.unite(id, (i - 1) * size + j);
                if (j > 0 && img[i][j - 1] == 1) uf.unite(id, i * size + (j - 1));
            }
        }
    }

    #pragma omp parallel for collapse(2)
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            if (img[i][j] == 1)
                labels[i][j] = uf.find(i * size + j) + 1;
}

int main(int argc, char *argv[]) {
    int size = SIZE;

    if (argc == 2) size = atoi(argv[1]);

    vector<vector<int>> img = generarImagen(size);
    vector<vector<int>> labelsSerial(size, vector<int>(size, 0));
    vector<vector<int>> labelsParalelo(size, vector<int>(size, 0));

    etiquetadoSerial(img, labelsSerial);
    etiquetadoParalelo(img, labelsParalelo);

    cout << "Etiquetado Serial vs Paralelo: " << (labelsSerial == labelsParalelo ? "OK" : "ERROR") << endl;

    return 0;
}
