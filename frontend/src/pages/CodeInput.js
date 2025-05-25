import React, { useState, useRef, useEffect } from "react";
import { useCodeMirror } from "@uiw/react-codemirror";
import { javascript } from "@codemirror/lang-javascript";
import { oneDark } from "@codemirror/theme-one-dark";

const CodeInput = ({ onVisualize }) => {
  const [code, setCode] = useState("");
  const editor = useRef();

  const { setContainer } = useCodeMirror({
    container: editor.current,
    extensions: [javascript()],
    theme: oneDark,
    value: code,
    height: "auto",
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
      body: JSON.stringify({ code }), // Send only the code
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

  return (
    <div className="container mt-4">
      <h2 className="mb-4 text-center">Code Input</h2>
      <div className="code-editor-container" ref={editor}></div>
      <div className="d-flex justify-content-end mt-3">
        <button onClick={handleVisualize} className="btn btn-primary">
          Visualize
        </button>
      </div>
    </div>
  );
};

export default CodeInput;
