import os

def parse_functions(filename, contents):
    functions = []
    lines = contents.split('\n')
    # Iterate through all the lines in the file
    for i, line in enumerate(lines):
        # Find function declarations that start at position 0
        if line: 
            # Check if the line starts with "static"
            if line.startswith("static"):
                # Add "//" to the start of the line
                line = "//" + line
            # Check if the last character in the line is a backslash
            if line[-1] == '\\' or line[-1] == ',':
                # If the last character is a backslash, include the next line as if it was part of the current line
                next_line = lines[i+1]
                line += next_line
            if line[0].isalpha() or line[0] == '*':
                # Extract the function name and its arguments
                function_name, *args = line.split()
                # Concatenate the function name and arguments and add a semicolon at the end
                function = ' '.join([function_name] + args) + ';'
                # Add the function to the list of functions
                functions.append(function)
    return functions


def main():
    directory = 'srcs'

    # Iterate through all the .c files in the directory
    for filename in sorted(os.listdir(directory)):
        if not filename.endswith('.c'):
            continue

        # Read the contents of the file
        with open(os.path.join(directory, filename), 'r') as file:
            contents = file.read()

        # Parse the file for functions
        functions = parse_functions(filename, contents)

        # Print the name of the file and the list of functions
        print(f'\n//{filename}')
        if functions:
            for function in functions:
                print(function)
        else:
            print('//No functions found')


if __name__ == '__main__':
    main()
