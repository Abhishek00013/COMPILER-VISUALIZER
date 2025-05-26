import React from "react";

const Contact = () => {
  return (
    <div className="contact-wrapper">
      <div className="contact-card">
        <h2>Contact Information</h2>
        <p><strong>Name:</strong> Priya Bahuguna</p>
        <p>
          <strong>Email:</strong>{" "}
          <a href="mailto:priyabahuguna220@gmail.com">
            priyabahuguna220@gmail.com
          </a>
        </p>
        <p><strong>Phone Number:</strong> +91-8193122457</p>
      </div>
    </div>
  );
};

export default Contact;
