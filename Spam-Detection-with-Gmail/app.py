from flask import Flask
from scripts.email_fetch import fetch_latest_email, classify_email
from_email, subject, body = fetch_latest_email()
result = classify_email(body)

app = Flask(__name__)
@app.route("/")
def home():
    email_data = fetch_latest_email()  
    if email_data:
        from_email, subject, body = email_data
        classification = classify_email(body)

        if classification == "Spam":
            image_path = "/static/images/spam.jpg"
            result_text = "Test failed: This is a spam email. Don't open this message!!"
            text_color = "red"  
        else:
            image_path = "/static/images/not_spam.jpg"
            result_text = "Test passed: This is not a spam email. You can open it!"
            text_color = "green"  

        # Render HTML
        html_content = f"""
        <!DOCTYPE html>
        <html>
        <head>
            <title>Email Classification</title>
            <style>
                body {{
                    font-family: Arial, sans-serif;
                    text-align: center;
                    margin: 20px;
                }}
                #image-container {{
                    display: none; /* Initially hidden */
                    margin-top: 20px;
                }}
                #result-text {{
                    display: none; /* Initially hidden */
                    font-size: 18px;
                    margin-top: 20px;
                    color: {text_color}; /* Dynamic color based on classification */
                }}
                #loading {{
                    font-size: 18px;
                    color: #555;
                }}
            </style>
            <script>
                function showImage() {{
                    // Simulate image loading delay
                    setTimeout(() => {{
                        document.getElementById("loading").style.display = "none";
                        document.getElementById("image-container").style.display = "block";
                        showText();
                    }}, 2000); // 2 seconds delay
                }}

                function showText() {{
                    // Gradually display the result text
                    const textElement = document.getElementById("result-text");
                    const text = textElement.getAttribute("data-text");
                    textElement.style.display = "block";
                    let i = 0;
                    const interval = setInterval(() => {{
                        textElement.textContent += text.charAt(i);
                        i++;
                        if (i >= text.length) clearInterval(interval);
                    }}, 40); // Adjust speed here (50ms per character)
                }}
            </script>
        </head>
        <body onload="showImage()">
            <h1>Email Classification</h1>
            <p><strong>From:</strong> {from_email}</p>
            <p><strong>Subject:</strong> {subject}</p>
            <p><strong>Body:</strong> {body}</p>
            <h2>Classification Result:</h2>
            <div id="loading">Generating result... Please wait.</div>
            <div id="image-container">
                <img src="{image_path}" alt="{classification}" style="width:300px;height:auto;">
            </div>
            <div id="result-text" data-text="{result_text}"></div>
        </body>
        </html>
        """
    else:
        html_content = """
        <!DOCTYPE html>
        <html>
        <head>
            <title>Email Classification</title>
        </head>
        <body>
            <h1>No unread emails found.</h1>
        </body>
        </html>
        """

    return html_content

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000, debug=True)