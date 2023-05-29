import os
Import("env")

env.Replace(PROJECT_DATA_DIR=os.path.join("$PROJECT_DIR", "data-by-env", "$PIOENV"))

