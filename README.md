<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
</head>
<body>

  <h1 class="emoji">🧭 Pathfinder Visualization</h1>

  <p>
    An interactive <strong>pathfinding algorithm visualizer</strong> built using <strong>C and OpenGL</strong>. It allows users to set start/end points, add obstacles, and visualize 
    <strong>BFS</strong>, <strong>Dijkstra</strong>, and <strong>A*</strong> algorithms in real-time on a 2D grid with smooth animations and user interaction.
  </p>

  <h2 class="emoji">🚀 Features</h2>
  <ul>
    <li>🎮 Mouse-controlled <strong>grid interaction</strong></li>
    <li>🧠 Supports 3 major algorithms:
      <ul>
        <li><strong>BFS</strong> (Breadth-First Search)</li>
        <li><strong>Dijkstra's Algorithm</strong></li>
        <li><strong>A*</strong> (A-Star) Algorithm</li>
      </ul>
    </li>
    <li>🔄 <strong>Animated pathfinding</strong> with adjustable speed</li>
    <li>🖥️ Real-time rendering using <strong>OpenGL</strong></li>
    <li>🧱 Diagonal movement enabled (but no corner cutting through obstacles)</li>
    <li>📏 Displays coordinates, algorithm, and path length in the GUI</li>
  </ul>

  <h2 class="emoji">🖼️ Demo (optional)</h2>
  <p><em>(Insert GIF or screenshot here)</em></p>

  <h2 class="emoji">📦 Requirements</h2>
  <ul>
    <li>OpenGL (via GLUT)</li>
    <li>C/C++ compiler (GCC, Clang, etc.)</li>
  </ul>

  <h2 class="emoji">🛠️ Compilation</h2>

  <p><strong>Linux (GCC):</strong></p>
  <pre><code>g++ pathfinder.cpp -o pathfinder -lGL -lGLU -lglut
./pathfinder</code></pre>

  <p><strong>Windows (MinGW or Visual Studio):</strong></p>
  <pre><code>g++ pathfinder.cpp -o pathfinder -lfreeglut -lopengl32 -lglu32
pathfinder.exe</code></pre>

  <h2 class="emoji">🎮 Controls</h2>
  <table>
    <thead>
      <tr>
        <th>Key/Mouse</th>
        <th>Action</th>
      </tr>
    </thead>
    <tbody>
      <tr><td>Left Click</td><td>Set <strong>Start</strong>, then <strong>End</strong>, then toggle obstacles</td></tr>
      <tr><td><code>s</code></td><td>Start pathfinding using selected algorithm</td></tr>
      <tr><td><code>r</code></td><td>Reset grid</td></tr>
      <tr><td><code>1</code></td><td>Select <strong>BFS</strong></td></tr>
      <tr><td><code>2</code></td><td>Select <strong>Dijkstra</strong></td></tr>
      <tr><td><code>3</code></td><td>Select <strong>A*</strong></td></tr>
    </tbody>
  </table>

  <h2 class="emoji">📋 Algorithm Details</h2>
  <ul>
    <li><strong>BFS:</strong> Explores uniformly, not optimal for diagonal cost</li>
    <li><strong>Dijkstra:</strong> Guarantees shortest path with uniform cost</li>
    <li><strong>A*:</strong> Uses heuristic (Manhattan distance) to optimize search</li>
  </ul>

  <h2 class="emoji">📚 Future Improvements</h2>
  <ul>
    <li>Add terrain weights (sand, water, etc.)</li>
    <li>Export path to a file</li>
    <li>Add curved/3D map support</li>
    <li>Graphical UI for controls</li>
  </ul>

  <h2 class="emoji">🧠 Credits</h2>
  <p>Built by <strong><a href="https://github.com/raghav-287" target="_blank">Raghav Singla</a></strong>.</p>

</body>
</html>
