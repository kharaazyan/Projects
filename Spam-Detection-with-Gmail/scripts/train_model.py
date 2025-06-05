import pandas as pd
from sklearn.feature_extraction.text import TfidfVectorizer
from sklearn.model_selection import train_test_split
from sklearn.naive_bayes import MultinomialNB
import joblib

data = pd.read_csv("/Users/arturterteryan/Desktop/Project/data/spam.csv", encoding="latin-1")[["v1", "v2"]]
data.columns = ["label", "message"]

data["label"] = data["label"].map({"spam": 1, "ham": 0})

X_train, X_test, y_train, y_test = train_test_split(
    data["message"], data["label"], test_size=0.2, random_state=42
)

vectorizer = TfidfVectorizer(stop_words="english")
X_train_vec = vectorizer.fit_transform(X_train)
X_test_vec = vectorizer.transform(X_test)

model = MultinomialNB()
model.fit(X_train_vec, y_train)

accuracy = model.score(X_test_vec, y_test)
print(f"Model Accuracy: {accuracy * 100:.2f}%")

joblib.dump(model, "/Users/arturterteryan/Desktop/Project/models/spam_model.pkl")
joblib.dump(vectorizer, "/Users/arturterteryan/Desktop/Project/models/vectorizer.pkl")
