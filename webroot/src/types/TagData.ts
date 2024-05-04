export interface PlayState {
    index?: number;
    position?: number;
    seed?: number;
}

export interface TagData {
    tag: string;
    name?: string;
    action?: string;
    contentRef?: string;
    savePlayState?: boolean;
    playState?: PlayState;
    randomize?:boolean;
    playCount?: number;
    lastSeen?:number; 
  }
  