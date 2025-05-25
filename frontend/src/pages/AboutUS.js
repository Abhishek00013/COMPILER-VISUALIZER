import React from "react";
import contributor1 from "../assets/contributors/contributor1.png";
import contributor2 from "../assets/contributors/contributor2.png";
import contributor3 from "../assets/contributors/contributor3.png";

const AboutUS = () => {
  return (
    <div className="container mt-4">
      <h2>About Us</h2>
      <p>
        Welcome to the Mini Compiler project! This application is designed to
        simulate the various phases of a compiler, including lexical analysis,
        syntax analysis, intermediate code generation, code optimization, and
        tagret code geneation.
      </p>
      <p>
        Our goal is to provide an educational tool for students and developers
        to understand how compilers work under the hood.
      </p>
      <p>
        Feel free to explore the application, input your code, and visualize the
        different phases of compilation.
      </p>

      <h3 className="mt-5">Contributors</h3>
      <div className="row mt-3">
        <div className="col-md-4 text-center">
          <img
            src={contributor1}
            alt="Contributor 1"
            className="img-fluid rounded-circle mb-2"
            style={{ width: "150px", height: "150px" }}
          />
          <h5>Abhishek Patwal</h5>
        </div>
        <div className="col-md-4 text-center">
          <img
            src={contributor2}
            alt="Contributor 2"
            className="img-fluid rounded-circle mb-2"
            style={{ width: "150px", height: "150px" }}
          />
          <h5>Navalpreet Kaur</h5>
        </div>
        <div className="col-md-4 text-center">
          <img
            src={contributor3}
            alt="Contributor 3"
            className="img-fluid rounded-circle mb-2"
            style={{ width: "150px", height: "150px" }}
          />
          <h5>Priya Bahuguna</h5>
        </div>
      </div>
    </div>
  );
};

export default AboutUS;
