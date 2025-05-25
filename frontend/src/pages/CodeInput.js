import React, { useState, useRef, useEffect } from "react";
import { useCodeMirror } from "@uiw/react-codemirror";
import { javascript } from "@codemirror/lang-javascript";
import { oneDark } from "@codemirror/theme-one-dark";

const CodeInput = ({ onVisualize }) => {
  const [code, setCode] = useState("");
  const [system, setSystem] = useState("windows"); // Default to Windows
  const editor = useRef();

  const { setContainer } = useCodeMirror({
    container: editor.current,
    extensions: [javascript()],
    theme: oneDark,
    value: code,
    height: "400px",
    onChange: (value) => {
      setCode(value);
    },
  });

  useEffect(() => {
    if (editor.current) {
      setContainer(editor.current);
    }
  }, [setContainer]);

  const handleVisualize = () => {
    if (!code.trim()) {
      alert("Please enter some code before visualizing.");
      return;
    }

    fetch("http://localhost:3001/compile", {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
      },
      body: JSON.stringify({ code, system }), // Send code and system
    })
      .then((response) => {
        if (!response.ok) {
          return response.text().then((text) => {
            throw new Error(text || `HTTP error! status: ${response.status}`);
          });
        }
        return response.json();
      })
      .then((data) => {
        if (onVisualize) {
          // Adjust the data format to match what the backend returns
          const outputs = [
            { phase: "Tokens", content: data.tokens },
            { phase: "Parse Tree", content: data.tree },
            { phase: "Intermediate Representation", content: data.ir },
            { phase: "Optimized IR", content: data.irOptimized },
            { phase: "Assembly Code", content: data.asm },
          ];
          onVisualize(outputs);
        }
      })
      .catch((error) => {
        console.error("Error:", error.message);
        alert(`Compilation error: ${error.message}`);
      });
  };

  const handleSystemChange = (event) => {
    setSystem(event.target.value);
  };

  return (
    <div className="container mt-4">
      <h2 className="mb-4 text-center">Code Input</h2>
      <div className="code-editor-container" ref={editor}></div>
      <div className="d-flex align-items-center justify-content-between mt-3">
  <div>
    <div className="form-check form-check-inline">
      <input
        className="form-check-input"
        type="radio"
        name="system"
        id="windows"
        value="windows"
        checked={system === "windows"}
        onChange={handleSystemChange}
        style={{ width: "24px", height: "24px", cursor: "pointer" }}
      />
      <label
        className="form-check-label"
        htmlFor="windows"
        style={{ fontSize: "1.5rem", marginLeft: "8px", cursor: "pointer", userSelect: "none" }}
      >
        Windows
      </label>
    </div>
    <div className="form-check form-check-inline">
      <input
        className="form-check-input"
        type="radio"
        name="system"
        id="mac"
        value="mac"
        checked={system === "mac"}
        onChange={handleSystemChange}
        style={{ width: "24px", height: "24px", cursor: "pointer" }}
      />
      <label
        className="form-check-label"
        htmlFor="mac"
        style={{ fontSize: "1.5rem", marginLeft: "8px", cursor: "pointer", userSelect: "none" }}
      >
        Mac
      </label>
    </div>
  </div>

  <button onClick={handleVisualize} className="btn btn-primary visualize-button">
    Visualize
  </button>
</div>

    </div>
  );
};

export default CodeInput;
