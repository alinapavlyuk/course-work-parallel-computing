import requests
import re

BASE_URL = "http://localhost:8002"


def highlight_words(content, search_words):
    if not search_words:
        return content

    highlight_start = '\033[43m'
    highlight_end = '\033[0m'

    regex_pattern = r'\b(' + '|'.join(map(re.escape, search_words)) + r')\b'

    def replace_match(match):
        return f"{highlight_start}{match.group(0)}{highlight_end}"

    highlighted_content = re.sub(regex_pattern, replace_match, content, flags=re.IGNORECASE)
    return highlighted_content


def fetch_search_results(query):
    formatted_query = "+".join(query.split())
    response = requests.get(f"{BASE_URL}/search?q={formatted_query}")

    if response.status_code != 200:
        print("Failed to fetch search results.")
        return []

    data = response.json()
    file_ids = data.get("fileIDs", [])[:15]  # Limit to 15 results
    if not file_ids:
        print("No results found.")
        return []

    return file_ids


def fetch_file_content(file_id):
    file_response = requests.get(f"{BASE_URL}/download?id={file_id}")
    if file_response.status_code == 200:
        return file_response.json()
    else:
        print(f"Failed to fetch file with ID {file_id}")
        return None


def search_files():
    query = input("Enter your search keywords: ").strip()
    if not query:
        print("Search query cannot be empty.")
        return []

    search_words = query.split()
    file_ids = fetch_search_results(query)

    if not file_ids:
        return []

    files = []
    for file_id in file_ids:
        file_data = fetch_file_content(file_id)
        if file_data:
            highlighted_content = highlight_words(file_data.get("content", ""), search_words)
            files.append({
                "fileID": file_id,
                "fileName": file_data.get("fileName", "Unknown"),
                "content": highlighted_content
            })

    print("\nSearch Results:")
    for idx, file in enumerate(files, start=1):
        print(f"{idx}. Name: {file['fileName']}")
    print()

    return files


def view_file(files):
    try:
        index = int(input("Enter the number of the file to view: ").strip())
        if index < 1 or index > len(files):
            print("Invalid number.")
            return

        file = files[index - 1]
        print(f"\nFile: {file['fileName']}")
        print(file['content'])
        print("-" * 80)
    except ValueError:
        print("Please enter a valid number.")


def menu():
    while True:
        print("\nMenu:")
        print("1. Search files by keywords")
        print("2. Download file by ID")
        print("3. Quit")
        choice = input("Enter your choice: ").strip()

        if choice == "1":
            files = search_files()
            if not files:
                continue

            while True:
                print("\nOptions:")
                print("1. View a file from the search results")
                print("2. Search again")
                print("3. Quit")
                sub_choice = input("Enter your choice: ").strip()

                if sub_choice == "1":
                    view_file(files)
                elif sub_choice == "2":
                    break
                elif sub_choice == "3":
                    print("Bye.")
                    return
                else:
                    print("Invalid choice. Please try again.")

        elif choice == "2":
            file_id = input("Enter the file ID to download: ").strip()
            file_data = fetch_file_content(file_id)
            if file_data:
                print(f"\nName: {file_data.get('fileName', 'Unknown')}")
                print(file_data.get('content', ''))
                print("-" * 80)

        elif choice == "3":
            print("Bye.")
            break

        else:
            print("Invalid choice. Please try again.")


if __name__ == "__main__":
    menu()
