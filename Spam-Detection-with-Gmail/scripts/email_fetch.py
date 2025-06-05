import joblib
import imaplib
import email
from email.header import decode_header

EMAIL = "your_email"
PASSWORD = "your_password"  
IMAP_SERVER = "imap.gmail.com"

def fetch_latest_email():
    mail = imaplib.IMAP4_SSL(IMAP_SERVER)
    mail.login(EMAIL, PASSWORD)
    mail.select("inbox")  

    _, search_data = mail.search(None, "UNSEEN")
    email_ids = search_data[0].split()  

    if not email_ids:
        return None, None, None  

    email_ids = [int(e_id) for e_id in email_ids]
    latest_email_id = str(max(email_ids))

    _, message_data = mail.fetch(latest_email_id, "(BODY.PEEK[])")

    raw_email = message_data[0][1].decode("utf-8")
    msg = email.message_from_string(raw_email)

    subject = decode_header(msg["Subject"])[0][0]
    if isinstance(subject, bytes):
        subject = subject.decode("utf-8")
    from_email = msg["From"]
    body = ""
    if msg.is_multipart():
        for part in msg.walk():
            if part.get_content_type() == "text/plain":
                body = part.get_payload(decode=True).decode("utf-8", errors="replace")
                break
    else:
        body = msg.get_payload(decode=True).decode("utf-8", errors="replace")

    return from_email, subject, body

from_email, subject, body = fetch_latest_email()
print("From:", from_email)
print("Subject:", subject)
print("Body:", body)


def classify_email(content):
    model = joblib.load("/Users/arturterteryan/Desktop/Project/models/spam_model.pkl")
    vectorizer = joblib.load("/Users/arturterteryan/Desktop/Project/models/vectorizer.pkl")

    transformed_content = vectorizer.transform([content])

    prediction = model.predict(transformed_content)
    return "Spam" if prediction[0] == 1 else "Not Spam"

from_email, subject, body = fetch_latest_email()
result = classify_email(body)
print(f"Email from {from_email} classified as: {result}")
