# g_features.py

def get_feature(name):
    features = {
        "feature1": "Description of feature 1",
        "feature2": "Description of feature 2",
        "feature3": "Description of feature 3"
    }
    return features.get(name, "Feature not found")
