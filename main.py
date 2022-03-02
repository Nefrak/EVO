from GA import GA
import yaml

"""
Main function
"""
def main():
  with open('params.yaml') as f:
    docs = yaml.load_all(f, Loader=yaml.FullLoader)
    for doc in docs:
        for k, v in doc.items():
            print(k, "->", v)

main()