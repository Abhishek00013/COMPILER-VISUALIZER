import React, { useState } from "react";

const CompilerPhases = ({ outputs }) => {
  const [currentIndex, setCurrentIndex] = useState(0);

  // Handle "Next" button click
  const handleNext = () => {
    setCurrentIndex((prevIndex) => (prevIndex + 1) % outputs.length);
  };

  // Handle "Previous" button click
  const handlePrevious = () => {
    setCurrentIndex(
      (prevIndex) => (prevIndex - 1 + outputs.length) % outputs.length
    );
  };

  if (outputs.length === 0) {
    return (
      <div className="container mt-4 text-center">
        <h2>Compiler Phases</h2>
        <p>No outputs to display. Please run the compiler first.</p>
      </div>
    );
  }

  return (
    <div className="container mt-4 text-center">
      <h2>Compiler Phases</h2>
      <div className="flashcard-container">
        {outputs.map((output, index) => (
          <div
            key={index}
            className={`flashcard ${
              index === currentIndex ? "active" : "inactive"
            }`}
          >
            <h3>{output.phase}</h3>
            {output.phase === "Parse Tree" ? (
              <img
                src="http://localhost:3001/compilerPhases/tree.png"
                alt="Parse Tree"
                className="img-fluid border rounded"
                style={{ maxWidth: "100%", height: "auto" }}
              />
            ) : (
              <pre className="p-3 rounded">{output.content}</pre>
            )}
          </div>
        ))}
      </div>
      <div className="navigation-buttons mt-3">
        <button className="btn btn-outline-primary" onClick={handlePrevious}>
          Previous
        </button>
        <button className="btn btn-outline-primary ml-2" onClick={handleNext}>
          Next
        </button>
      </div>
    </div>
  );
};

export default CompilerPhases;
