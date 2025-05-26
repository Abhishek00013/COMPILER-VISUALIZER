import React from "react";
import contributor1 from "../assets/contributors/contributor1.png";
import contributor2 from "../assets/contributors/contributor2.png";
import contributor3 from "../assets/contributors/contributor3.png";

const AboutUS = () => {
  return (
    <div className="container mt-4">
      <h2>About Us</h2>
      <p>
        Welcome to the Mini Compiler Visualizer - an interactive educational tool designed to simulate the inner workings of a compiler. This project walks you through key phases of compilation such as:
        Lexical Analysis
        Syntax Analysis
        Intermediate Code Generation
        Code Optimization
        Target Code Generation
      </p>
      <p>
        Our mission is to make compiler concepts more accessible and intuitive for students, educators, and curious developers alike. Whether you're just starting out or brushing up on core computer science principles, this application provides a hands-on way to explore how compilers process and transform code.
      </p>
      <p>
        Feel free to contact us. You can contact us through the form on our Contact Us page.
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
