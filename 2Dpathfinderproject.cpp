#include <GL/glut.h>
#include <vector>
#include <queue>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <sstream> 
#include <map>
#include <iomanip> 

#define GRID_SIZE 25
#define WINDOW_SIZE 800
#define ANIMATION_SPEED 50 // Milliseconds per step

struct Node {
    int x, y;
    bool obstacle = false;
    bool visited = false;
    float gCost = INFINITY;
    float hCost = 0.0f;
    float fCost = INFINITY;
    Node* parent = nullptr;
};

Node grid[GRID_SIZE][GRID_SIZE];
Node* startNode = nullptr;
Node* endNode = nullptr;
std::vector<Node*> path;
bool findingPath = false;
bool pathFound = false; // Flag to indicate if a path was found
float pathLength = 0.0f;

// Animation variables
std::vector<Node*> visitedNodesAnimation;
int animationIndex = 0;
bool animatingPath = false;

// Algorithm selection
enum Algorithm { BFS, DIJKSTRA, ASTAR };
Algorithm currentAlgo = ASTAR;

void setAlgorithm(Algorithm algo) {
    currentAlgo = algo;
    std::cout << "Algorithm set to: ";
    if (currentAlgo == BFS) std::cout << "BFS" << std::endl;
    else if (currentAlgo == DIJKSTRA) std::cout << "Dijkstra" << std::endl;
    else if (currentAlgo == ASTAR) std::cout << "A*" << std::endl;
}

void initializeGrid() {
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            grid[i][j].x = i;
            grid[i][j].y = j;
            grid[i][j].obstacle = false;
            grid[i][j].visited = false;
            grid[i][j].gCost = INFINITY;
            grid[i][j].hCost = 0.0f;
            grid[i][j].fCost = INFINITY;
            grid[i][j].parent = nullptr;
        }
    }
    startNode = nullptr;
    endNode = nullptr;
    path.clear();
    findingPath = false;
    pathFound = false;
    pathLength = 0.0f;
    visitedNodesAnimation.clear();
    animationIndex = 0;
    animatingPath = false;
}

// Heuristic function for A*
float calculateHeuristic(Node* a, Node* b) {
    return std::abs(a->x - b->x) + std::abs(a->y - b->y); // Manhattan distance
}

void reconstructPath(Node* current) {
    Node* temp = current;
    path.clear();
    pathLength = 0.0f;
    while (temp != nullptr) {
        path.push_back(temp);
        if (temp->parent) {
            pathLength += std::sqrt(std::pow(temp->x - temp->parent->x, 2) + std::pow(temp->y - temp->parent->y, 2));
        }
        temp = temp->parent;
    }
    std::reverse(path.begin(), path.end());
}

void findPath() {
    if (!startNode || !endNode) {
        std::cout << "Start or end node not set." << std::endl;
        return;
    }

    // Reset visited status and costs for all nodes
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            grid[i][j].visited = false;
            grid[i][j].parent = nullptr;
            grid[i][j].gCost = INFINITY;
            grid[i][j].fCost = INFINITY;
        }
    }
    path.clear();
    pathLength = 0.0f;
    visitedNodesAnimation.clear();
    findingPath = true;
    pathFound = false;
    animatingPath = false;
    animationIndex = 0;

    startNode->gCost = 0;
    visitedNodesAnimation.push_back(startNode);
    std::queue<Node*> bfsQueue;
    std::priority_queue<std::pair<float, Node*>, std::vector<std::pair<float, Node*> >, std::greater<std::pair<float, Node*> > > dijkstraQueue;

    if (currentAlgo == BFS) {
        bfsQueue.push(startNode);
        startNode->visited = true;
    } else if (currentAlgo == DIJKSTRA || currentAlgo == ASTAR) {
        startNode->fCost = 0;
        dijkstraQueue.push({0, startNode});
    }

    while ((currentAlgo == BFS && !bfsQueue.empty()) || ((currentAlgo == DIJKSTRA || currentAlgo == ASTAR) && !dijkstraQueue.empty())) {
        Node* current = nullptr;
        if (currentAlgo == BFS) {
            current = bfsQueue.front();
            bfsQueue.pop();
        } else {
            current = dijkstraQueue.top().second;
            dijkstraQueue.pop();
        }

        if (current == endNode) {
            reconstructPath(current);
            findingPath = false;
            pathFound = true;
            animatingPath = true;
            animationIndex = 0;
            glutPostRedisplay();
            return;
        }

        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if (dx == 0 && dy == 0) continue;

                int nx = current->x + dx;
                int ny = current->y + dy;

                if (nx >= 0 && nx < GRID_SIZE && ny >= 0 && ny < GRID_SIZE) {
                    Node* neighbor = &grid[nx][ny];

                    if (neighbor->obstacle) continue;

                    // Prevent diagonal movement through corners
                    if (std::abs(dx) == 1 && std::abs(dy) == 1) {
                        if (nx - dx >= 0 && nx - dx < GRID_SIZE && grid[nx - dx][ny].obstacle) continue;
                        if (ny - dy >= 0 && ny - dy < GRID_SIZE && grid[nx][ny - dy].obstacle) continue;
                    }

                    float moveCost = std::sqrt(dx * dx + dy * dy);
                    float newGCost = current->gCost + moveCost;

                    if (currentAlgo == BFS) {
                        if (!neighbor->visited) {
                            neighbor->visited = true;
                            neighbor->parent = current;
                            bfsQueue.push(neighbor);
                            visitedNodesAnimation.push_back(neighbor);
                        }
                    } else if (currentAlgo == DIJKSTRA || currentAlgo == ASTAR) {
                        float tentativeGCost = current->gCost + moveCost;
                        if (tentativeGCost < neighbor->gCost) {
                            neighbor->gCost = tentativeGCost;
                            neighbor->hCost = (currentAlgo == ASTAR) ? calculateHeuristic(neighbor, endNode) : 0;
                            neighbor->fCost = neighbor->gCost + neighbor->hCost;
                            neighbor->parent = current;
                            dijkstraQueue.push({neighbor->fCost, neighbor});
                            visitedNodesAnimation.push_back(neighbor);
                        }
                    }
                }
            }
        }
    }

    std::cout << "Path not found." << std::endl;
    findingPath = false;
    glutPostRedisplay();
}

void drawText(float x, float y, const std::string& text, float r, float g, float b) {
    glColor3f(r, g, b);
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
    }
}

// OpenGL rendering functions
void drawGrid() {
    float cellSize = WINDOW_SIZE / (float)GRID_SIZE;
    glBegin(GL_QUADS);
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            if (grid[i][j].obstacle)
                glColor3f(0.2, 0.2, 0.2); // Obstacle color
            else if (findingPath && std::find(visitedNodesAnimation.begin(), visitedNodesAnimation.end(), &grid[i][j]) != visitedNodesAnimation.end() && !animatingPath)
                glColor3f(0.8, 0.8, 1.0); // Visited color during search (before animation)
            else
                glColor3f(1.0, 1.0, 1.0); // Default grid color

            glVertex2f(i * cellSize, j * cellSize);
            glVertex2f((i + 1) * cellSize, j * cellSize);
            glVertex2f((i + 1) * cellSize, (j + 1) * cellSize);
            glVertex2f(i * cellSize, (j + 1) * cellSize);
        }
    }
    glEnd();

    // Draw grid lines
    glColor3f(0.7, 0.7, 0.7);
    glLineWidth(1.0);
    glBegin(GL_LINES);
    for (int i = 0; i <= GRID_SIZE; ++i) {
        glVertex2f(i * cellSize, 0);
        glVertex2f(i * cellSize, WINDOW_SIZE);
        glVertex2f(0, i * cellSize);
        glVertex2f(WINDOW_SIZE, i * cellSize);
    }
    glEnd();

    // Draw start and end nodes
    float cellSizeHalf = cellSize * 0.5;
    float startEndRadius = cellSize * 0.3;
    int circleSegments = 16;

    if (startNode) {
        glColor3f(0.0, 1.0, 0.0); // Green for start
        float centerX = (startNode->x + 0.5) * cellSize;
        float centerY = (startNode->y + 0.5) * cellSize;
        glBegin(GL_POLYGON);
        for (int i = 0; i < circleSegments; ++i) {
            float theta = 2.0f * M_PI * float(i) / float(circleSegments);
            float x = startEndRadius * cosf(theta);
            float y = startEndRadius * sinf(theta);
            glVertex2f(centerX + x, centerY + y);
        }
        glEnd();
    }

    if (endNode) {
        glColor3f(1.0, 0.0, 0.0); // Red for end
        float centerX = (endNode->x + 0.5) * cellSize;
        float centerY = (endNode->y + 0.5) * cellSize;
        glBegin(GL_POLYGON);
        for (int i = 0; i < circleSegments; ++i) {
            float theta = 2.0f * M_PI * float(i) / float(circleSegments);
            float x = startEndRadius * cosf(theta);
            float y = startEndRadius * sinf(theta);
            glVertex2f(centerX + x, centerY + y);
        }
        glEnd();
    }

    // Draw the animated path
    if (animatingPath && !path.empty()) {
        glColor3f(1.0, 1.0, 0.0); // Yellow for path
        glLineWidth(3.0);
        glBegin(GL_LINE_STRIP);
        for (int i = 0; i <= animationIndex && i < path.size(); ++i) {
            float centerX = (path[i]->x + 0.5) * cellSize;
            float centerY = (path[i]->y + 0.5) * cellSize;
            glVertex2f(centerX, centerY);
        }
        glEnd();
        glLineWidth(1.0);
    } else if (pathFound && !path.empty()) {
        // Draw the final path if animation is not active
        glColor3f(1.0, 1.0, 0.0); // Yellow for path
        glLineWidth(3.0);
        glBegin(GL_LINE_STRIP);
        for (const auto& node : path) {
            float centerX = (node->x + 0.5) * cellSize;
            float centerY = (node->y + 0.5) * cellSize;
            glVertex2f(centerX, centerY);
        }
        glEnd();
        glLineWidth(1.0);
    }

    // Display information
    std::stringstream ss;
    ss << "Click to set/switch Start (Green), then End (Red), then Obstacles (Black).";
    drawText(10, WINDOW_SIZE - 20, ss.str(), 0.0, 0.0, 0.0);

    ss.str("");
    ss << "Press 's' to start pathfinding. Press 'r' to reset.";
    drawText(10, WINDOW_SIZE - 40, ss.str(), 0.0, 0.0, 0.0);

    ss.str("");
    ss << "Algorithm: ";
    if (currentAlgo == BFS) ss << "BFS";
    else if (currentAlgo == DIJKSTRA) ss << "Dijkstra";
    else if (currentAlgo == ASTAR) ss << "A*";
    drawText(10, WINDOW_SIZE - 60, ss.str(), 0.0, 0.0, 0.0);

    ss.str("");
    ss << "Press '1' for BFS, '2' for Dijkstra, '3' for A*.";
    drawText(10, WINDOW_SIZE - 80, ss.str(), 0.0, 0.0, 0.0);

    ss.str("");
    ss << "Movement: 8 directions (including diagonals). Diagonal movement through corners is blocked.";
    drawText(10, WINDOW_SIZE - 100, ss.str(), 0.0, 0.0, 0.0);

    if (startNode) {
        ss.str("");
        ss << "Start: (" << startNode->x << ", " << startNode->y << ")";
        drawText(10, 20, ss.str(), 0.0, 0.7, 0.0);
    }

    if (endNode) {
        ss.str("");
        ss << "End: (" << endNode->x << ", " << endNode->y << ")";
        drawText(10, 40, ss.str(), 0.7, 0.0, 0.0);
    }

    if (pathFound) {
        ss.str("");
        ss << "Path Length: " << std::fixed << std::setprecision(2) << pathLength;
        drawText(10, 80, ss.str(), 0.0, 0.0, 0.0);
    }

    if (pathFound && animatingPath) {
        drawText(10, 60, "Animating Path...", 0.0, 0.0, 0.0);
    } else if (pathFound) {
        drawText(10, 60, "Path Found!", 1.0, 1.0, 0.0);
    } else if (findingPath) {
        drawText(10, 60, "Finding Path...", 0.8, 0.8, 1.0);
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawGrid();
    glutSwapBuffers();
}

void timer(int value) {
    if (animatingPath && pathFound) {
        animationIndex++;
        if (animationIndex >= path.size()) {
            animatingPath = false; // Stop animation when end is reached
        }
        glutPostRedisplay();
        glutTimerFunc(ANIMATION_SPEED, timer, 0); // Continue animation
    }
}

// Mouse interaction handling
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        int gridX = (x * GRID_SIZE) / WINDOW_SIZE;
        int gridY = ((WINDOW_SIZE - y) * GRID_SIZE) / WINDOW_SIZE; 

        if (gridX >= 0 && gridX < GRID_SIZE && gridY >= 0 && gridY < GRID_SIZE) {
            Node* clickedNode = &grid[gridX][gridY];

            if (startNode == clickedNode) {
                std::cout << "Start node deselected." << std::endl;
                startNode = nullptr;
                path.clear();
                pathFound = false;
                pathLength = 0.0f;
                animatingPath =false;
                animationIndex = 0;
                glutPostRedisplay();
            } else if (endNode == clickedNode) {
                std::cout << "End node deselected." << std::endl;
                endNode = nullptr;
                path.clear();
                pathFound = false;
                pathLength = 0.0f;
                animatingPath = false;
                animationIndex = 0;
                glutPostRedisplay();
            } else if (!startNode) {
                startNode = clickedNode;
                std::cout << "Start node set at: (" << gridX << ", " << gridY << ")" << std::endl;
                glutPostRedisplay();
            } else if (!endNode) {
                endNode = clickedNode;
                std::cout << "End node set at: (" << gridX << ", " << gridY << ")" << std::endl;
                glutPostRedisplay();
            } else {
                clickedNode->obstacle = !clickedNode->obstacle;
                std::cout << "Toggled obstacle at: (" << gridX << ", " << gridY << ")" << (clickedNode->obstacle ? " (on)" : " (off)") << std::endl;
                path.clear();
                pathFound = false;
                pathLength = 0.0f;
                animatingPath = false;
                animationIndex = 0;
                glutPostRedisplay();
            }
        }
    }
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 's') {
        if (startNode && endNode) {
            std::cout << "Finding path using ";
            if (currentAlgo == BFS) std::cout << "BFS";
            else if (currentAlgo == DIJKSTRA) std::cout << "Dijkstra";
            else if (currentAlgo == ASTAR) std::cout << "A*";
            std::cout << "..." << std::endl;
            findPath();
            if (pathFound) {
                glutTimerFunc(ANIMATION_SPEED, timer, 0); // Start animation timer
            }
        } else {
            std::cout << "Please set start and end nodes first." << std::endl;
        }
    } else if (key == 'r') {
        std::cout << "Resetting grid." << std::endl;
        initializeGrid();
        glutPostRedisplay();
    } else if (key == '1') {
        setAlgorithm(BFS);
    } else if (key == '2') {
        setAlgorithm(DIJKSTRA);
    } else if (key == '3') {
        setAlgorithm(ASTAR);
    }
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_SIZE, WINDOW_SIZE);
    glutCreateWindow("Pathfinder Visualization");

    glClearColor(0.9, 0.9, 0.9, 0.0); // Light gray background
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WINDOW_SIZE, 0, WINDOW_SIZE);
    glMatrixMode(GL_MODELVIEW);

    initializeGrid();
    setAlgorithm(BFS); // Default to BFS

    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard); 
    glutMainLoop();
    return 0;
}