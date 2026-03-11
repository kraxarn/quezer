#!/bin/python
# Checks for updates in third-party libraries

import os
import typing

import httpx

success = True

http = httpx.Client(headers={
	"Accept": "application/vnd.github.v3+json",
})

token = os.getenv("GITHUB_TOKEN")
if token:
	http.headers.update({
		"Authorization": f"token {token}"
	})


def log(lib_name: str, current_version: str, latest_version: str):
	if current_version == latest_version:
		print(f" {lib_name} is up-to-date ({current_version})")
	else:
		print(f" {lib_name} {latest_version} is available (current is {current_version})")
		global success
		success = False


def latest_tag(repo_name: str) -> str:
	tags = http.get(f"https://api.github.com/repos/{repo_name}/tags").json()
	for tag in tags:
		if "-" not in tag["name"]:
			return tag["name"]
	return ""


# .github/workflows

def qt_version_from_workflow(filename: str) -> typing.Optional[str]:
	with open(filename, "r") as file:
		for line in file:
			if "QT_VERSION" in line:
				return "v{0}".format(line[line.index("\"") + 1:line.rindex("\"")])
	return None


# deps

def repository_from_dep(filename: str) -> typing.Optional[str]:
	with open(filename, "r") as file:
		for line in file:
			if "GIT_REPOSITORY" in line:
				return line[line.index("GIT_REPOSITORY") + 15:].strip()
	return None


def tag_from_dep(filename: str) -> typing.Optional[str]:
	with open(filename, "r") as file:
		for line in file:
			if "GIT_TAG" in line:
				return line[line.index("GIT_TAG") + 8:].strip()
	return None


latest_qt = latest_tag("qt/qtbase")
workflows_dir = os.fsencode(".github/workflows/")

for file in os.listdir(workflows_dir):
	basename = file.decode()
	file_path = os.path.join(workflows_dir, file).decode()
	version = qt_version_from_workflow(file_path)
	if version is None:
		continue
	log(f"Qt ({basename})", version, latest_qt)

actions = {}

for filename in os.listdir(workflows_dir):
	basename = filename.decode()
	file_path = os.path.join(workflows_dir, filename).decode()
	with open(file_path, "r") as file:
		for line in file:
			if "uses:" in line:
				action = line[line.index("uses:") + 5:].strip().split("@")
				if action[0] in actions.keys() and action[1] != actions[action[0]]:
					print(f" Mismatch {action[0]}: {action[1]} != {actions[action[0]]}")
				else:
					actions[action[0]] = action[1]

for action_name, action_version in actions.items():
	repo_name = "/".join(action_name.split("/")[:2])
	tag = latest_tag(repo_name)
	action = tag if tag.startswith("v0") or "." not in tag else tag[:tag.index(".")]
	log(action_name, action_version, action)

for file in os.listdir("deps"):
	filename = os.path.join("deps", file)
	repo = repository_from_dep(filename)
	tag = tag_from_dep(filename)
	name = repo[19:len(repo) - 4]
	latest_version = latest_tag(name)
	log(name, tag, latest_version)

http.close()
exit(0 if success else 1)
