# ICFPC2018 問題

## 概要

ナノボットを適切に動かして3次元空間上に与えられたオブジェを作ろう！  
重力があるので、普通にやると下から素材を積み上げていく感じになるけど、なんと無重力状態に切り替えることができるのです！！

- ナノボットは常に浮遊して移動できる(多分)
- ナノボットは分裂や融合ができる
- ナノボットは単位立方体voxelに素材を注入できる
- 素材の入ったvoxelには動けない
- フィールドの外には動けない
- ナノボット同士が干渉し合う動きも出来ない(ぶつかるとか)
- フィールド( $R\times R\times R$ 立方体)には重力があるけど `harmonics` を`High`にすれば無重力にできる
- 重力状態で浮いてる(後述)素材はあってはならない

## 用語の定義や準備

### 座標系

普通の3次元直交座標です  
左から右にx軸、下から上にy軸、手前から奥にz軸が伸びる(左手系なので注意)

### 座標

$0 \lt R \le 250$  
フィールドは $[0,R-1]\times [0,R-1]\times [0,R-1]$ のvoxelで構成されてることを表す  
一つの座標が一つのvoxelに対応していて、左下手前の隅っこにあるvoxelが $(0,0,0)$です  
(まぁわかるよね)

### 変位(*Coordinate difference*)

って書いてあるやつ。$d = (dx,dy,dz)$ のこと

- $mlen(d) = |dx|+|dy|+|dz|$
- $clen(d) = \max\{|dx|,|dy|,|dz|\}$

- 座標$c, c'$が「隣接」 $\overset{\mathrm{def}}{\Leftrightarrow}$ $mlen(c'-c) = 1$

#### 直線的変位(*Linear Coordinate difference*)

- $d$が「直線的($ld$と特記することにする)」 $\overset{\mathrm{def}}{\Leftrightarrow}$ $d = (dx,0,0) \lor (0,dy,0) \lor (0,0,dz)$   ($dx,dy,dz \gt 0$)
- $ld$が「短い直線的変位($sld$)」 $\overset{\mathrm{def}}{\Leftrightarrow}$ $mlen(ld) \le 5$
- $ld$が「長い直線的変位($lld$)」 $\overset{\mathrm{def}}{\Leftrightarrow}$ $mlen(ld) \le 15$

#### 近傍変位(*Near Coordinate difference*)

- $d$が近傍変位($nd$) $\overset{\mathrm{def}}{\Leftrightarrow}$ $mlen(d) \le 2 ~\land~ clen(d) = 1$  

18近傍

#### 遠方変位(*Far Coordinate difference*)
- $d$が遠方変位($fd$) $\overset{\mathrm{def}}{\Leftrightarrow}$ $mlen(d) \le 2 ~\land~ clen(d) = 1$  

#### 領域(Region)

座標$c, c'$の張る直方体領域を$[c1,c2]$と書く  
$c = (x,y,z), c' = (x',y',z')$とすると

- $[c1,c2] = [\min\{x,x'\},\max\{x,x'\}]\times [\min\{y,y'\},\max\{y,y'\}]\times [\min\{z,z'\},\max\{z,z'\}]$

#### 領域の次元(Dimension of a region)
$r =[(x1,y1,z1),(x2,y2,z2)]$とした時、
-$dim(r) =  (x1 = x2 ? 0 : 1) + (y1 = y2 ? 0 : 1) + (z1 = z2 ? 0 : 1)$
つまり$r$が立体なら3で平面なら2みたいな感じ

## Matrix

フィールドの各voxelに対して`Full`または`Empty`という2状態がある  
Matrixは$[0,R-1]\times[0,R-1]\times[0,R-1]$の各voxelの状態を表す3次元0-1行列みたいな感じ  
(行列と言うか、フィールドの状態を表す3次元配列)  

### 接地している

`Full`なvoxelに対して、「接地している」という状態を以下のように定義する

- $y=0$のvoxelは接地している
- ある近傍の`Full`かつ接地したvoxelが存在するなら、接地している

要するに、地面と連結

## システム状態とコマンド実行

### システムの状態

システム全体$S$は以下のパラメータ(状態)を持つ

- `energy` : 消費エネルギー(最終的に小さいほど良い結果)
- `harmonics`: 浮遊しているか(`Low` or `High`)
- `matrix`: 現在の物質の埋まり具合
- `bots`: 現在居るナノボット(集合)
- `trace`: コマンド列

各ナノボットは以下のパラメータを持つ

- `bid`: 固有の番号
- `pos`: 現在いる座標
- `seeds`: 分裂時に使える番号(後述)、子供の番号みたいに思えば良いかも

システムが「適切な状態(well-formed)」であることは以下を満たすことである

- `harmonics`が`Low`なら、全ての`Full`なvoxelは接地している
- 全てのナノボットの番号が異なる
- 全てのナノボットの`seeds`はdisjoint
- 全てのナノボットの位置は異なり、かつ`Empty`なvoxel
- 任意のナノボットの`seeds`の中に、現在存在するナノボットの番号が含まれていない(わかりづらいけど余り深く考える必要は無さそう)

### コマンド実行

各ターンでどのようにナノボットが動くかを説明する  

1. システムが「適切」でないなら`Error`
2. $S.trace$の先頭から一つづつボットに命令を割り当てる
3. 実行(ここでも$S.energy$は変動)
4. 実行したコマンドを$S.trace$から削除
5. $S.energy$の加算

2の割り当て方を詳しく説明する  
このターンで存在するナノボットを$S.bots = \{bot_1,\dots,bot_n\}   (bot_1.bid \lt \dots \lt bot_n.bid)$とする  
ここで$S.trace$を$cmd_1,cmd_2,\dots,cmd_n,\dots$とおくとき、$cmd_i$を$bot_i$に割り当てる($S.trace$が長さ$n$未満なら`Error`)

3 については後にコマンド一覧でまとめる

4での$S.energy$の変化には2種類ある

``` text
    if(S.harmonics == High)
        S.energy += 30 * R * R * R
    else
        S.energy += 3 * R * R * R
    endif
```

``` text
    S.energy += 20 * n // nは存在するナノボット数
```

### 衝突

実行中にナノボット同士が衝突しうるようなコマンドは`Error`となる。これを以下のように言い換える。  

>各実行コマンドと実行する位置によって、「使用する(volatile)」voxel集合が定義される
>これが全てのナノボットについて重ならないような実行のみ可能である。

## コマンド一覧

以下、

- $bot$: コマンド実行をするナノボット
- $c = bot.pos$

とする。

### 単独コマンド

- Halt (終了コマンド)
    - 「$c \neq (0,0,0) \land S.bots=\{bot\} \land S.harmonics \neq Low$ 」なら`Error`  
    - 「使用領域」は$\{c\}$  
    - $S.bots = \{\}$にする効果がある
    - コストなし

- Wait (待機)
    - エラー条件なし
    - 「使用領域」は$\{c\}$
    - 効果なし
    - コストなし

- Flip (重力切り替え)
    - エラー条件なし
    - 「使用領域」は$\{c\}$
    - $S.harmonics$を切り替える
    - コストなし

- SMove $lld$ (直線移動,長い移動)  
    $c' = c+lld$とする
    - 「$c'$がフィールド外 $~\lor~$ $[c,c']$に`Full`なvoxelが含まれる」なら`Error`
    - 「使用領域」は$[c,c']$
    - $bot.pos = c'$となる。移動ね
    - コスト: $2\times mlen(lld)$, つまり$S.energy = S.energy+2*mlen(lld)$

- LMove $sld_1$ $sld_2$ (L字移動,短い移動２つ)  
    $c' = c+sld_1$, $c'' = c' + sld_2$とする
    - 「$c',c''$がフィールド外 $~\lor~$ $[c,c']\cup [c',c'']$に`Full`なvoxelが含まれる」なら`Error`
    - 「使用領域」は$[c,c']\cup [c',c'']$
    - $bot.pos = c''$となる。移動ね
    - コスト:$2*(mlen(sld_1)+2+mlen(sld_2))$

- Fission $nd$ $m$ (近傍に分裂を生成, $m$個の`seed`を渡す)
    $c' = c+nd$とし、$bot.seeds = \{bid1_1,\dots,bid_n\}$(昇順)とする
    - 「$bot.seeds = \{\} ~\lor~ c'$がフィールド外$~\lor~ c'$が`Full` $~\lor~~ n \le m$」なら`Error`
    - 「使用領域」は$\{c,c'\}$
    - 効果は以下の通り
    ```text
        bots.seed = { bid_{m+2} ,..., bid_n }
        bot'.bid = bid_1
        bot'.pos = c'
        bot'.seeds = { bid_2 ,..., bid_{m+1} }
        S.bots = union(S.bots, {bot'})
    ```
    - コスト: $24$

- Fill $nd$ (voxelに物体を注入する)  
    $c' = c+nd$とする
    - $c'$がフィールド外なら`Error`
    - 「使用領域」は$\{c,c'\}$
    - 効果は$c'$を`Full`にする
    - コスト:$c'$がもともと`Full`なら$6$, そうでないなら$12$

- Void $nd$ (voxelの物体を取り除く)
    $c' = c+nd$とする
    - $c'$がフィールド外なら`Error`
    - 「使用領域」は$\{c,c'\}$
    - 効果は$c'$を`Void`にする
    - コスト:$c'$が`Full`なら$-12$, そうでないなら$3$
### グループコマンド

- FusionP $nd_p$ と FusionS $nd_s$ (融合コマンド。FusionPをしたボットとFusionSをボットで融合)  
    - $bot_p.pos + nd_p, bot_s.pos + nd_s$がフィールド外なら`Error`
    さらに、
    - 「使用領域」は$\{bot_p.pos,bot_s.pos\}$
    - $bot_p.pos + nd_p = bot_s.pos ~\land~ bot_s.pos+nd_s = bot_p.pos$の場合のみ以下の効果がある
    ```text
       S.bots = diff(S.bots, { bot_s })
       bot_p.seeds = union(bot_p.seeds, {bot_s.bid}, bot_s.seeds)
    ```
    - コスト: -24
- GFill
$r$を領域とし、$n=dim(r)$とする。
     - $n$個のbotがいて$bot_i$はGFill $nd_i ,fd_i$を$r = [bot_i.pos+nd_i,bot_i.pos+nd_i+fd_i]$となるように実行。
効果としてはこの領域内の各voxelに対してFillをすることができる。
    - bot_i.pos+ndi,bot_i.pos+ndi+fdiがフィールド外なら`Error`
    - さらに、bot_i.pos+ndi = bot_j.pos+ndjとなる時や、bot_i.posが$r$内にある時も`Error`
    - Void voxelに対してはコスト$12$で、Full Voxelだとコスト$6$。
    - 使用領域は各$bot_i.pos$と$r$
- GVoid
 GFillの逆でコストはFull voxelに対しては$-12$でVoid voxelは$3$