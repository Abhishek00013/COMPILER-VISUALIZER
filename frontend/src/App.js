import React, { useState } from "react";
import { Route, Routes, useNavigate } from "react-router-dom";
import Navbar from "./components/Navbar";
import CodeInput from "./pages/CodeInput";
import CompilerPhases from "./pages/CompilerPhases";
import AboutUS from "./pages/AboutUS";

function App() {
  const [system, setSystem] = useState("");
  const [outputs, setOutputs] = useState([]);
  const navigate = useNavigate();

  const handleSystemSelect = (selectedSystem) => {
    setSystem(selectedSystem);
    console.log("Selected system:", selectedSystem);
  };

  const handleVisualize = async (code) => {
    try {
      const res = await fetch("http://localhost:3001/compile", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ code, system }),
      });
      const data = await res.json();

      setOutputs([
        { phase: "Tokens", content: data.tokens },
        { phase: "Parse Tree", content: data.tree },
        { phase: "Intermediate Representation", content: data.ir },
        { phase: "Optimized IR", content: data.irOptimized },
        { phase: "Assembly Code", content: data.asm },
      ]);

      navigate("/compiler-phases");
    } catch (err) {
      console.error("Error during visualization:", err);
    }
  };

  return (
    <>
      <Navbar />
      <Routes>
        <Route
          path="/"
          element={
            <CodeInput
              onSystemSelect={handleSystemSelect}
              onVisualize={handleVisualize}
            />
          }
        />
        <Route
          path="/compiler-phases"
          element={<CompilerPhases outputs={outputs} />}
        />
        <Route path="/about" element={<AboutUS />} />
      </Routes>
    </>
  );
}

export default App;
