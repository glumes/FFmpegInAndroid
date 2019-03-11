package com.glumes.ffmpeginandroid;
//
//public class GenreAdapter extends ExpandableRecyclerViewAdapter<GenreViewHolder, ArtistViewHolder> {
//
//  public GenreAdapter(List<? extends ExpandableGroup> groups) {
//    super(groups);
//  }
//
//  @Override
//  public GenreViewHolder onCreateGroupViewHolder(ViewGroup parent, int viewType) {
//    View view = LayoutInflater.from(parent.getContext())
//        .inflate(R.layout.list_item_genre, parent, false);
//    return new GenreViewHolder(view);
//  }
//
//  @Override
//  public ArtistViewHolder onCreateChildViewHolder(ViewGroup parent, int viewType) {
//    View view = LayoutInflater.from(parent.getContext())
//        .inflate(R.layout.list_item_artist, parent, false);
//    return new ArtistViewHolder(view);
//  }
//
//  @Override
//  public void onBindChildViewHolder(ArtistViewHolder holder, int flatPosition,
//      ExpandableGroup group, int childIndex) {
//
//    final Artist artist = ((Genre) group).getItems().get(childIndex);
//    holder.setArtistName(artist.getName());
//  }
//
//  @Override
//  public void onBindGroupViewHolder(GenreViewHolder holder, int flatPosition,
//      ExpandableGroup group) {
//
//    holder.setGenreTitle(group);
//  }
//}