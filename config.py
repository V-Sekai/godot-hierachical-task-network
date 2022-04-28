# config.py

def can_build(env, platform):
    return env["platform"] != "osx"

def configure(env):
    pass

def get_doc_path():
    return "doc_classes"

def get_doc_classes():
    return [
        "TaskPlanner",
    ]
