#!/usr/bin/python3
import os
import json
import subprocess

PROJECT_FILE_NAME = "project.json"


def get_project():
    project = {}
    with open(PROJECT_FILE_NAME) as project_file:
        project = json.load(project_file)
    return project


def run_command_string(cmd):
    cmd_list = cmd.split(' ',)
    subprocess.call(cmd_list, cwd=os.getcwd())


def run_clear(project, platform):
    cmd = project['platforms'][platform]['steps']['clear']
    cmd = cmd.replace('$build-dir', project['build-dir'])
    run_command_string(cmd)


def compile_source(source, project, platform):
    includes = project['platforms'][platform]['includes']
    build_dir = f"{project['build-dir']}/{platform}"
    pre_compile_cmd = project['platforms'][platform]['steps']['pre-compile']
    pre_compile_cmd = pre_compile_cmd.replace(
        '$object-dir', f"{build_dir}/{os.path.dirname(source)}")
    run_command_string(pre_compile_cmd)
    object = f"{build_dir}/{source}.o"
    compile_cmd = project['platforms'][platform]['steps']['compile']
    compile_cmd = compile_cmd.replace('$source', source)
    compile_cmd = compile_cmd.replace('$includes', " ".join(includes))
    compile_cmd = compile_cmd.replace('$object', object)
    run_command_string(compile_cmd)
    return object


def link_all(project, platform, objects):
    build_dir = f"{project['build-dir']}/{platform}"
    target = f"{build_dir}/{project['name']}"
    link_cmd = project['platforms'][platform]['steps']['link']
    link_cmd = link_cmd.replace('$objects', " ".join(objects))
    link_cmd = link_cmd.replace('$target', target)
    run_command_string(link_cmd)


def main():
    project = get_project()
    project_name = project['name']
    build_dir = project['build-dir']
    for platform in project['platforms']:
        objects = []
        run_clear(project, platform)
        sources = project['platforms'][platform]['sources']
        for source in sources:
            objects.append(compile_source(source, project, platform))
        link_all(project, platform, objects)


if __name__ == "__main__":
    main()
