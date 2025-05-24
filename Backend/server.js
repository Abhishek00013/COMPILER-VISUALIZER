const express = require("express");
const fs = require("fs");
const path = require("path");
const { exec } = require("child_process");
const cors = require("cors");

const app = express();
const PORT = 3001;

const phasesDir = path.join(__dirname, "../compilerPhases");
const inputPath = path.join(phasesDir, "input.txt");

app.use(cors());
app.use(express.json());

// Serve static files from ../public
app.use(express.static(path.join(__dirname, "../public")));

// Compile route
app.post("/compile", (req, res) => {
  const code = req.body.code;
  if (!code) return res.status(400).send("No code provided");

  fs.writeFileSync(inputPath, code);

  const compileCommand = `gcc main.c lexer.c parser.c semantic.c ir.c optimizer.c codegen.c -o compiler.exe`;
  const runCommand = `compiler.exe`;

  exec(compileCommand, { cwd: phasesDir }, (err, stdout, stderr) => {
    if (err) return res.status(500).send("Compilation error:\n" + stderr);

    exec(runCommand, { cwd: phasesDir }, (err2, stdout2, stderr2) => {
      if (err2) return res.status(500).send("Execution error:\n" + stderr2);

      const filesToRead = [
        "tokens.txt",
        "tree.dot",
        "ir.txt",
        "ir_optimized.txt",
        "output.asm",
      ];

      const results = {};
      try {
        for (const file of filesToRead) {
          const filePath = path.join(phasesDir, file);
          results[file] = fs.readFileSync(filePath, "utf-8");
        }
      } catch (readErr) {
        return res.status(500).send(`Execution succeeded but missing output file: ${readErr.message}`);
      }

      res.json({
        tokens: results["tokens.txt"],
        tree: results["tree.dot"],
        ir: results["ir.txt"],
        irOptimized: results["ir_optimized.txt"],
        asm: results["output.asm"],
      });
    });
  });
});

// Optional: send index.html on root
app.get("/", (req, res) => {
  res.sendFile(path.join(__dirname, "../public/index.html"));
});

app.listen(PORT, () => {
  console.log(`✅ Server running at http://localhost:${PORT}`);
});
