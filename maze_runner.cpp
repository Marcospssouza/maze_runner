#include <stdio.h>
#include <iostream>
#include <stack>
#include <vector>

// Matriz de char representnado o labirinto
char** maze; // Voce também pode representar o labirinto como um vetor de vetores de char (vector<vector<char>>)

// Numero de linhas e colunas do labirinto
int num_rows;
int num_cols;

// Representação de uma posição
struct pos_t {
	int i;
	int j;
};

// Estrutura de dados contendo as próximas
// posicões a serem exploradas no labirinto
std::stack<pos_t> valid_positions;

pos_t load_maze(const char* file_name) {
	pos_t initial_pos;
    // Abre o arquivo para leitura (fopen)
	FILE* file = fopen(file_name, "r");
    if (!file) {
		printf("Erro ao abrir o arquivo do labirinto. %s\n", file_name);
    	exit(1); 
    }
	// Le o numero de linhas e colunas (fscanf) e salva em num_rows e num_cols
	fscanf(file, "%d %d", &num_rows, &num_cols);
 	printf("%d linhas e %d colunas\n",num_cols,num_rows);

	// Aloca a matriz maze (malloc)
	maze = (char **)malloc(num_rows * sizeof(char *));
  	if (maze == NULL) {
    printf("Erro em alocar a matriz\n");
    exit(1);
  	}
  	for (int i = 0; i < num_rows; ++i) {
    maze[i] = (char *)malloc(num_cols * sizeof(char));
    if (maze[i] == NULL) {
      printf("Erro em alocar a matriz\n");
      exit(1);
    }
  	}

  for (int i = 0; i < num_rows; ++i) {
        for (int j = 0; j < num_cols; ++j) {
            char cell;
            maze[i][j] = cell;
            if (cell == 'e') {
                initial_pos.i = i;
                initial_pos.j = j;
            }
        }
    }
    fclose(file);
    return initial_pos;
}

// Função que imprime o labirinto
void print_maze() {
	for (int i = 0; i < num_rows; ++i) {
		for (int j = 0; j < num_cols; ++j) {
			printf("%c", maze[i][j]);
		}
		printf("\n");
	}
}

void check(pos_t next_pos) {
    int directions[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}}; // Direções: direita, esquerda, baixo, cima
    
    for (int i = 0; i < 4; i++) {
        int newRow = next_pos.i + directions[i][0];
        int newCol = next_pos.j + directions[i][1];
        
        if (newRow >= 0 && newRow < num_rows && newCol >= 0 && newCol < num_cols && maze[newRow][newCol] == 's') {
            maze[next_pos.i][next_pos.j] = '.';
            print_maze();
            printf("Found the exit!\n");
            exit(0);
        }
    }
}

// Função responsável pela navegação.
// Recebe como entrada a posição initial e retorna um booleando indicando se a saída foi encontrada
bool walk(pos_t pos) {
    maze[pos.i][pos.j] = '.';

    int directions[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}}; // Direções: direita, esquerda, baixo, cima
    
    for (int i = 0; i < 4; i++) {
        int newRow = pos.i + directions[i][0];
        int newCol = pos.j + directions[i][1];
        
        if (newRow >= 0 && newRow < num_rows && newCol >= 0 && newCol < num_cols && maze[newRow][newCol] == 'x') {
            pos_t next_pos = {newRow, newCol};
            valid_positions.push(next_pos);
            maze[next_pos.i][next_pos.j] = 'o';
        }
    }

    print_maze();
    return false;
}


int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s <file_name>\n", argv[0]);
        return 1;
    }
    
    pos_t initial_pos = load_maze(argv[1]);

    print_maze();

    valid_positions.push(initial_pos);

    while (!valid_positions.empty()) {
        pos_t pos = valid_positions.top();
        valid_positions.pop();
        if (walk(pos)) {
            printf("Found the exit!\n");
            return 0;
        }
    }

    printf("Could not find the exit!\n");

    return 0;
}
