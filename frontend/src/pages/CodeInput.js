import React, { useState } from "react";
import { useCodeMirror } from "@uiw/react-codemirror";
import { javascript } from "@codemirror/lang-javascript";
import { oneDark } from "@codemirror/theme-one-dark";

const CodeInput = ({ onSystemSelect, onVisualize }) => {
  const [code, setCode] = useState("");
  const [system, setSystem] = useState("");

  const handleSystemSelect = (selectedSystem) => {
    setSystem(selectedSystem);
    if (onSystemSelect) {
      onSystemSelect(selectedSystem);
    }
  };

  const handleVisualize = () => {
    if (!system) {
      alert("Please select a system (Windows or Mac) first.");
      return;
    }
    if (onVisualize) {
      onVisualize(code);
    }
  };

  const { setContainer } = useCodeMirror({
    value: code,
    extensions: [javascript()],
    theme: oneDark,
    onChange: (value) => setCode(value),
  });

  return (
    <div className="container mt-4">
      <h2 className="mb-4 text-center">Code Input</h2>
      <div className="code-editor-container" ref={setContainer}></div>
      <div className="d-flex justify-content-between mt-3">
        <div>
          <button
            onClick={() => handleSystemSelect("windows")}
            className={`btn ${
              system === "windows" ? "btn-primary" : "btn-outline-primary"
            } mr-2`}
          >
            Windows
          </button>
          <button
            onClick={() => handleSystemSelect("mac")}
            className={`btn ${
              system === "mac" ? "btn-success" : "btn-outline-success"
            }`}
          >
            Mac
          </button>
        </div>
        <button onClick={handleVisualize} className={`btn btn-primary`}>
          Visualize
        </button>
      </div>
    </div>
  );
};

export default CodeInput;
