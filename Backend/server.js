const express = require("express");
const fs = require("fs");
const path = require("path");
const { exec } = require("child_process");
const cors = require("cors");

const app = express();
const PORT = process.env.PORT || 3001;

const phasesDir = path.join(__dirname, "../compilerPhases");
const inputPath = path.join(phasesDir, "input.txt");

app.use(cors());
app.use(express.json());

// Serve static files
app.use(express.static(path.join(__dirname, "../public")));
app.use("/compilerPhases", express.static(phasesDir));

// Compile endpoint
app.post("/compile", async (req, res) => {
  const { code, system = "windows" } = req.body;
  if (!code) return res.status(400).send("No code provided");

  try {
    fs.writeFileSync(inputPath, code);
  } catch (err) {
    return res.status(500).send("Failed to write code to file: " + err.message);
  }

  const isWindows = system === "windows";

  const compileCmd = isWindows
    ? `gcc main.c lexer.c parser.c semantic.c ir.c optimizer.c codegen.c -o compiler.exe`
    : `gcc main.c lexer.c parser.c semantic.c ir.c optimizer.c codegen.c -o compiler`;

  const runCmd = isWindows ? `compiler.exe` : `./compiler`;
  const dotCmd = isWindows ? `dot -Tpng tree.dot -o tree.png` : `dot -Tpng tree.dot -o tree.png`;

  // Step 1: Compile
  exec(compileCmd, { cwd: phasesDir }, (compileErr, _, compileStderr) => {
    if (compileErr) {
      return res.status(500).send("❌ Compilation failed:\n" + compileStderr);
    }

    // Step 2: Run
    exec(runCmd, { cwd: phasesDir }, (runErr, _, runStderr) => {
      if (runErr) {
        return res.status(500).send("❌ Execution failed:\n" + runStderr);
      }

      // Step 3: Generate tree.png from tree.dot
      exec(dotCmd, { cwd: phasesDir }, (dotErr, _, dotStderr) => {
        if (dotErr) {
          console.warn("⚠️ DOT to PNG conversion failed:\n" + dotStderr);
        }

        // Step 4: Read outputs
        const outputFiles = [
          "tokens.txt",
          "tree.dot",
          "ir.txt",
          "ir_optimized.txt",
          "output.asm",
        ];

        const results = {};
        try {
          for (const file of outputFiles) {
            const filePath = path.join(phasesDir, file);
            results[file] = fs.readFileSync(filePath, "utf-8");
          }
        } catch (readErr) {
          return res.status(500).send("Output reading error:\n" + readErr.message);
        }

        // Step 5: Send results
        res.json({
          tokens: results["tokens.txt"],
          tree: results["tree.dot"],
          ir: results["ir.txt"],
          irOptimized: results["ir_optimized.txt"],
          asm: results["output.asm"],
          treeImageUrl: "/compilerPhases/tree.png", // Accessible via <img src="">
        });
      });
    });
  });
});

// Root route
app.get("/", (req, res) => {
  res.sendFile(path.join(__dirname, "../public/index.html"));
});

app.listen(PORT, () => {
  console.log(`✅ Server running at http://localhost:${PORT}`);
});
