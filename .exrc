if &cp | set nocp | endif
let s:cpo_save=&cpo
set cpo&vim
imap <F4> \tp
map! <F1> <F1>
vnoremap  <
vmap 	 
nnoremap  :next
nnoremap  :prev
vnoremap  >
nnoremap <silent> w :CCTreeWindowToggle
nnoremap <silent> y :CCTreeWindowSaveCopy
noremap   <PageDown>
noremap - <PageUp>
vnoremap Q gq
nnoremap Q gqap
noremap Y y$
nnoremap \th :set invhls hls?
nnoremap \tl :set invlist list?
nnoremap \tp :set invpaste paste?
xmap gx <Plug>NetrwBrowseXVis
nmap gx <Plug>NetrwBrowseX
nnoremap <silent> <C-Bslash>w :CCTreeWindowToggle
nnoremap <silent> <C-Bslash>y :CCTreeWindowSaveCopy
nnoremap <silent> <F5> :!./zombie_friends
xnoremap <silent> <Plug>NetrwBrowseXVis :call netrw#BrowseXVis()
nnoremap <silent> <Plug>NetrwBrowseX :call netrw#BrowseX(netrw#GX(),netrw#CheckIfRemote(netrw#GX()))
nmap <F2> \tl
nmap <F4> \tp
vmap <S-Tab> 
vnoremap <C-D> <
vnoremap <C-T> >
omap <F1> <F1>
vmap <F1> <F1>
nnoremap <F1> :help 
nnoremap <C-P> :prev
nnoremap <C-N> :next
nnoremap <S-F6> W
nnoremap <F6> w
noremap <Del> 2
noremap <Insert> 2
noremap <BS> <PageUp>
let &cpo=s:cpo_save
unlet s:cpo_save
set autoindent
set background=dark
set backspace=eol,start,indent
set cindent
set comments=://,b:#,:%,:XCOMM,n:>,fb:-,s:/*,mb:**,ex:*/,fb:*,b:\",n::
set completeopt=menu,preview,menuone,noselect
set cscopeprg=/usr/bin/cscope
set cscopetag
set cscopeverbose
set display=truncate
set exrc
set fileencodings=ucs-bom,utf-8,default,latin1
set formatoptions=cqt
set gdefault
set guicursor=n-v-c:block-Cursor/lCursor,ve:ver35-Cursor,o:hor50-Cursor,i-ci:ver25-Cursor/lCursor,r-cr:hor20-Cursor/lCursor,sm:block-Cursor-blinkwait175-blinkoff150-blinkon175,a:blinkon0,i:hor20-Cursor/lCursor
set guifont=Monospace\ 12
set helplang=en
set hlsearch
set ignorecase
set incsearch
set langnoremap
set nolangremap
set laststatus=2
set listchars=eol:$,tab:»·
set matchpairs=(:),{:},[:],<:>
set nomodeline
set mouse=a
set nrformats=bin,hex
set pastetoggle=<F4>
set pumheight=10
set ruler
set runtimepath=~/.vim,~/.vim/plugged/nerdtree,~/.vim/plugged/vim-airline,~/.vim/plugged/gruvbox,~/.vim/plugged/vim-simple-complete,~/.vim/plugged/ayu-vim,~/.vim/plugged/gruvbox-material,~/.vim/plugged/night-owl.vim,~/.vim/plugged/nord-vim,~/.vim/plugged/vim-colors-meh,~/.vim/plugged/vim-cpp-modern,/usr/share/vim/vimfiles,/usr/share/vim/vim82,/usr/share/vim/vimfiles/after,~/.vim/plugged/vim-cpp-modern/after,~/.vim/after
set scrolloff=5
set secure
set shiftwidth=4
set shortmess=filnxtToOSr
set showcmd
set smartcase
set softtabstop=4
set tabstop=4
set termencoding=utf-8
set textwidth=72
set ttimeout
set ttimeoutlen=100
set viminfo=/10,'10,r/mnt/zip,r/mnt/floppy,f0,h,\"100
set whichwrap=h,l,~,[,]
set wildmenu
set wildmode=list:full
set window=38
" vim: set ft=vim :
