#!/usr/bin/env python3

import os
import requests
import subprocess
import sys
import dropbox

# Check for necessary access tokens
try:
    PR_number = os.environ['TRAVIS_PULL_REQUEST']
    if PR_number == 'false':
        # Skip PDF deployment on non-PR builds
        raise KeyError()
    repo_slug = os.environ['TRAVIS_REPO_SLUG']
    DB_ACCESS_TOKEN = os.environ['DROPBOX_ACCESS_TOKEN']
    GH_ACCESS_TOKEN = os.environ['GITHUB_ACCESS_TOKEN']
except KeyError as e:
    sys.stderr.write('Skipping PDF deployment...\n')
    exit(0)

with open('main_spec.pdf', mode='rb') as f:
    # Get the latest non-merge commit hash
    commit = subprocess.getoutput('git log -n 1 --format="%h" --no-merges')
    target = '/Travis-CI/openshmem-draft-{}.pdf'.format(commit)

    # Upload and share the PDF
    try:
        # Connect to Dropbox
        dbx = dropbox.Dropbox(DB_ACCESS_TOKEN)

        # Upload the PDF
        X = dbx.files_upload(f.read(), target,
                             mode=dropbox.files.WriteMode('overwrite', None))

        # Get a public URL for the PDF
        Y = dbx.sharing_create_shared_link_with_settings(target)
        pdfurl = Y.url.split('?')[0] + '?dl=1'
        comment = '[Draft PDF of this PR as of commit `{}`]({})'.format(commit, pdfurl)
    except dropbox.exceptions.DropboxException:
        sys.stderr.write('Failed to upload to / share via Dropbox\n')
        comment = 'Alert @nspark; the OpenSHMEM bot failed'

    # Post the URL comment to GitHub
    try:
        headers = {'Authorization': 'Bearer {}'.format(GH_ACCESS_TOKEN)}
        post_url = 'https://api.github.com/repos/{}/issues/{}/comments'.format(
            repo_slug, PR_number)
        print(requests.post(post_url, headers=headers, json={'body': comment},
                            timeout=1))
    except requests.exceptions.RequestException:
        sys.stderr.write('Failed to post comment to GitHub\n')
