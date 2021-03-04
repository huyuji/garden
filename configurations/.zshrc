# Enable Powerlevel10k instant prompt. Should stay close to the top of ~/.zshrc.
# Initialization code that may require console input (password prompts, [y/n]
# confirmations, etc.) must go above this block; everything else may go below.
if [[ -r "${XDG_CACHE_HOME:-$HOME/.cache}/p10k-instant-prompt-${(%):-%n}.zsh" ]]; then
  source "${XDG_CACHE_HOME:-$HOME/.cache}/p10k-instant-prompt-${(%):-%n}.zsh"
fi

source /usr/local/opt/powerlevel10k/powerlevel10k.zsh-theme

# To customize prompt, run `p10k configure` or edit ~/.p10k.zsh.
[[ ! -f ~/.p10k.zsh ]] || source ~/.p10k.zsh

# Load Git completion
zstyle ':completion:*:*:git:*' script ~/.zsh/git-completion.bash
fpath=(~/.zsh $fpath)

autoload -Uz compinit && compinit

cvi42root=/Users/yuji/Documents/code/cvi42
alias cmakerel="export SDKROOT=$(xcodebuild -version -sdk macosx Path); cmake -DCMR42_RELEASE=CMR42_RELEASE=1 -Bbuild -H. -DCMAKE_BUILD_TYPE=Release"
alias buildrel="cmake --build build --config Release -- -j 6"
alias runrel="open /Users/yuji/Documents/code/cvi42/build/production/Release/bin/cvi42.app"
alias runserverrel="open /Users/yuji/Documents/code/cvi42/build/production/Release/bin/cvi42Server.app"
alias watchrellog="tail -f /Users/yuji/Documents/code/cvi42/build/production/Release/bin/cvi42.app/Contents/MacOS/log.txt"
alias qmlwarningrel="grep -e \"qWarning.*\\.qml\" /Users/yuji/Documents/code/cvi42/build/production/Release/bin/cvi42.app/Contents/MacOS/log.txt"

alias cmakedebug="export SDKROOT=$(xcodebuild -version -sdk macosx Path); cmake -DCMR42_RELEASE=CMR42_RELEASE=1 -B../cvi42-debug -H. -DCMAKE_BUILD_TYPE=Debug"
alias builddebug="cmake --build ../cvi42-debug --config Debug -- -j 6"
alias rundebug="open /Users/yuji/Documents/code/cvi42-debug/production/Debug/bin/cvi42.app"
alias runserverdebug="open /Users/yuji/Documents/code/cvi42-debug/production/Debug/bin/cvi42Server.app"
alias watchdebuglog="tail -f /Users/yuji/Documents/code/cvi42-debug/production/Debug/bin/cvi42.app/Contents/MacOS/log.txt"
alias qmlwarningdebug="grep -e \"qWarning.*\\.qml\" /Users/yuji/Documents/code/cvi42-debug/production/Debug/bin/cvi42.app/Contents/MacOS/log.txt"

alias runwebserver="$cvi42root/webserver/cvi42_webserver &> ~/Documents/cvi42_webserver.log &!"

