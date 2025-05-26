import React, { useState } from "react";
import { Route, Routes, useNavigate } from "react-router-dom";
import Navbar from "./components/Navbar";
import CodeInput from "./pages/CodeInput";
import CompilerPhases from "./pages/CompilerPhases";
import AboutUS from "./pages/AboutUS";
import Contact from "./pages/Contact";
import "./App.css";
import 'normalize.css';
function App() {
  const [outputs, setOutputs] = useState([]);
  const navigate = useNavigate();

  const handleVisualize = (outputs) => {
    setOutputs(outputs);
    navigate("/compiler-phases");
  };

  return (
    <>
      <Navbar />
      <Routes>
        <Route
          path="/"
          element={<CodeInput onVisualize={handleVisualize} />}
        />
        <Route
          path="/compiler-phases"
          element={<CompilerPhases outputs={outputs} />}
        />
        <Route path="/about" element={<AboutUS />} />
        <Route path="/contact" element={<Contact />} />
      </Routes>
    </>
  );
}

export default App;
